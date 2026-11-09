#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <limits>
#include <map>

using namespace std;

// 收支记录类 - 用于存储单条财务记录的所有信息
class FinancialRecord {
private:
    string date;        // 日期，格式：YYYY-MM-DD
    string type;        // 类型："income"(收入) 或 "expense"(支出)
    double amount;      // 金额
    string category;    // 分类（如：工资、餐饮、交通等）
    string description; // 描述（可选）

public:
    // 构造函数：创建新的财务记录
    FinancialRecord(string d, string t, double a, string c, string desc = "")
        : date(d), type(t), amount(a), category(c), description(desc) {}

    // ========== Getter 方法 ==========
    // 获取日期
    string getDate() const {
        return date;
    }
    // 获取类型
    string getType() const {
        return type;
    }
    // 获取金额
    double getAmount() const {
        return amount;
    }
    // 获取分类
    string getCategory() const {
        return category;
    }
    // 获取描述
    string getDescription() const {
        return description;
    }

    // 显示记录信息 - 格式化输出到控制台
    void display() const {
        cout << date << " | " << setw(7) << type << " | "
             << setw(10) << amount << " | " << setw(15) << category
             << " | " << description << endl;
    }

    // 转换为字符串用于文件存储 - 将记录序列化为CSV格式
    string toString() const {
        return date + "," + type + "," + to_string(amount) + "," + category + "," + description;
    }

    // 从字符串创建记录 - 反序列化，从CSV格式恢复记录对象
    static FinancialRecord fromString(const string& data) {
        vector<string> tokens;  // 存储分割后的字段
        string token;
        istringstream tokenStream(data);

        // 按逗号分割字符串
        while (getline(tokenStream, token, ',')) {
            tokens.push_back(token);
        }

        // 验证数据完整性（至少需要4个字段）
        if (tokens.size() >= 4) {
            string desc = tokens.size() > 4 ? tokens[4] : "";  // 描述字段可选
            return FinancialRecord(tokens[0], tokens[1], stod(tokens[2]), tokens[3], desc);
        }
        throw invalid_argument("Invalid data format");  // 数据格式错误时抛出异常
    }
};

// 财务管理类 - 系统的核心类，负责管理所有财务记录
class FinanceManager {
private:
    vector<FinancialRecord> records;  // 存储所有财务记录的容器
    string filename;                   // 数据文件名
    double financialGoal;              // 财务自由目标金额

    // 检查日期有效性 - 验证输入的日期格式和逻辑正确性
    bool isValidDate(const string& date) {
        // 基本格式检查：必须是 YYYY-MM-DD 格式
        if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
            return false;
        }

        try {
            // 提取年、月、日
            int year = stoi(date.substr(0, 4));
            int month = stoi(date.substr(5, 2));
            int day = stoi(date.substr(8, 2));

            // 检查月份和日期的基本范围
            if (month < 1 || month > 12 || day < 1 || day > 31) {
                return false;
            }

            // 特殊处理2月：检查闰年和平年的天数
            if (month == 2) {
                bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                if (day > (isLeap ? 29 : 28)) {
                    return false;
                }
            }

            // 检查小月（4、6、9、11月）的天数限制
            if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
                return false;
            }

            return true;
        } catch (...) {
            return false;  // 转换失败时返回false
        }
    }

public:
    // 构造函数：初始化财务管理器，默认目标金额为100000
    FinanceManager(const string& file = "financial_data.txt") : filename(file), financialGoal(100000) {
        loadData();  // 启动时自动加载数据
    }

    // 析构函数：程序结束时自动保存数据
    ~FinanceManager() {
        saveData();
    }

    // 加载数据 - 从文件读取所有财务记录
    void loadData() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "No existing data file found. Starting fresh." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            try {
                records.push_back(FinancialRecord::fromString(line));
            } catch (const exception& e) {
                cerr << "Error parsing line: " << line << endl;  // 解析失败时输出错误信息
            }
        }
        file.close();
        cout << "Loaded " << records.size() << " records." << endl;
    }

    // 保存数据 - 将所有记录写入文件
    void saveData() {
        ofstream file(filename);
        for (const auto& record : records) {
            file << record.toString() << endl;
        }
        file.close();
        cout << "Data saved successfully." << endl;
    }

    // 添加记录 - 用户输入新记录的所有信息
    void addRecord(const string& type) {
        string date, category, description;
        double amount;

        // 输入并验证日期
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;
        while (!isValidDate(date)) {
            cout << "Invalid date! Please enter a valid date (YYYY-MM-DD): ";
            cin >> date;
        }

        // 输入并验证金额
        cout << "Enter amount: ";
        while (!(cin >> amount) || amount <= 0) {
            cout << "Invalid amount! Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(); // 清除换行符
        cout << "Enter category: ";
        getline(cin, category);

        cout << "Enter description (optional): ";
        getline(cin, description);

        // 创建新记录并添加到容器中
        records.emplace_back(date, type, amount, category, description);
        cout << "Record added successfully!" << endl;

        // 添加记录后检查收支平衡
        checkBalance();
    }

    // 检查收支平衡 - 提供智能提醒功能
    void checkBalance() {
        double balance = getBalance();

        if (balance < 0) {
            cout << "?  WARNING: You are overspending! Current balance: " << balance << endl;
        } else if (balance >= financialGoal) {
            cout << " CONGRATULATIONS! You've achieved financial freedom! Current balance: " << balance << endl;
        }
    }

    // 获取当前余额 - 计算总收入减去总支出
    double getBalance() const {
        double totalIncome = 0, totalExpense = 0;

        for (const auto& record : records) {
            if (record.getType() == "income") {
                totalIncome += record.getAmount();
            } else {
                totalExpense += record.getAmount();
            }
        }

        return totalIncome - totalExpense;
    }

    // 显示指定日期范围内的记录 - 支持按类型筛选
    void showRecordsByDateRange(const string& type) {
        string startDate, endDate;

        // 输入并验证开始日期
        cout << "Enter start date (YYYY-MM-DD): ";
        cin >> startDate;
        while (!isValidDate(startDate)) {
            cout << "Invalid date! Please enter a valid date (YYYY-MM-DD): ";
            cin >> startDate;
        }

        // 输入并验证结束日期
        cout << "Enter end date (YYYY-MM-DD): ";
        cin >> endDate;
        while (!isValidDate(endDate)) {
            cout << "Invalid date! Please enter a valid date (YYYY-MM-DD): ";
            cin >> endDate;
        }

        // 显示表头
        cout << "\nRecords from " << startDate << " to " << endDate << ":" << endl;
        cout << "Date       | Type    | Amount     | Category        | Description" << endl;
        cout << "------------------------------------------------------------------------" << endl;

        // 遍历所有记录，筛选符合条件的记录
        int count = 0;
        for (const auto& record : records) {
            if (record.getType() == type && record.getDate() >= startDate && record.getDate() <= endDate) {
                record.display();
                count++;
            }
        }

        if (count == 0) {
            cout << "No records found in the specified date range." << endl;
        }
    }

    // 修改记录 - 允许用户修改指定日期的记录
    void modifyRecord() {
        string date;
        cout << "Enter the date of the record to modify (YYYY-MM-DD): ";
        cin >> date;

        // 查找指定日期的所有记录
        vector<int> indices;
        for (int i = 0; i < records.size(); i++) {
            if (records[i].getDate() == date) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) {
            cout << "No records found for date: " << date << endl;
            return;
        }

        // 显示找到的记录供用户选择
        cout << "Found " << indices.size() << " records for date: " << date << endl;
        for (int i = 0; i < indices.size(); i++) {
            cout << i + 1 << ". ";
            records[indices[i]].display();
        }

        int choice;
        cout << "Select record to modify (1-" << indices.size() << "): ";
        cin >> choice;

        if (choice < 1 || choice > indices.size()) {
            cout << "Invalid selection!" << endl;
            return;
        }

        // 获取用户输入的新信息
        int index = indices[choice - 1];
        string type, category, description;
        double amount;

        cout << "Enter new type (income/expense): ";
        cin >> type;
        while (type != "income" && type != "expense") {
            cout << "Invalid type! Enter 'income' or 'expense': ";
            cin >> type;
        }

        cout << "Enter new amount: ";
        while (!(cin >> amount) || amount <= 0) {
            cout << "Invalid amount! Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore();
        cout << "Enter new category: ";
        getline(cin, category);

        cout << "Enter new description (optional): ";
        getline(cin, description);

        // 用新信息替换原记录
        records[index] = FinancialRecord(date, type, amount, category, description);
        cout << "Record modified successfully!" << endl;
    }

    // 删除记录 - 允许用户删除指定日期的记录
    void deleteRecord() {
        string date;
        cout << "Enter the date of the record to delete (YYYY-MM-DD): ";
        cin >> date;

        // 查找指定日期的所有记录
        vector<int> indices;
        for (int i = 0; i < records.size(); i++) {
            if (records[i].getDate() == date) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) {
            cout << "No records found for date: " << date << endl;
            return;
        }

        // 显示找到的记录供用户选择
        cout << "Found " << indices.size() << " records for date: " << date << endl;
        for (int i = 0; i < indices.size(); i++) {
            cout << i + 1 << ". ";
            records[indices[i]].display();
        }

        int choice;
        cout << "Select record to delete (1-" << indices.size() << "), or 0 to cancel: ";
        cin >> choice;

        if (choice == 0) {
            return;  // 用户取消删除
        }

        if (choice < 1 || choice > indices.size()) {
            cout << "Invalid selection!" << endl;
            return;
        }

        // 删除选中的记录
        int index = indices[choice - 1];
        records.erase(records.begin() + index);
        cout << "Record deleted successfully!" << endl;
    }

    // 统计类别收支 - 按日期范围和类别统计收支情况
    void categoryStatistics() {
        string startDate, endDate;

        // 输入并验证开始日期
        cout << "Enter start date (YYYY-MM-DD): ";
        cin >> startDate;
        while (!isValidDate(startDate)) {
            cout << "Invalid date! Please enter a valid date (YYYY-MM-DD): ";
            cin >> startDate;
        }

        // 输入并验证结束日期
        cout << "Enter end date (YYYY-MM-DD): ";
        cin >> endDate;
        while (!isValidDate(endDate)) {
            cout << "Invalid date! Please enter a valid date (YYYY-MM-DD): ";
            cin >> endDate;
        }

        // 使用map存储各类别的收支统计
        map<string, double> incomeByCategory;
        map<string, double> expenseByCategory;

        // 遍历指定日期范围内的所有记录
        for (const auto& record : records) {
            if (record.getDate() >= startDate && record.getDate() <= endDate) {
                if (record.getType() == "income") {
                    incomeByCategory[record.getCategory()] += record.getAmount();
                } else {
                    expenseByCategory[record.getCategory()] += record.getAmount();
                }
            }
        }

        // 显示收入分类统计
        cout << "\nIncome by category from " << startDate << " to " << endDate << ":" << endl;
        for (const auto& pair : incomeByCategory) {
            cout << pair.first << ": " << pair.second << endl;
        }

        // 显示支出分类统计
        cout << "\nExpense by category from " << startDate << " to " << endDate << ":" << endl;
        for (const auto& pair : expenseByCategory) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }

    // 显示所有记录 - 显示系统中的所有财务记录
    void showAllRecords() const {
        if (records.empty()) {
            cout << "No records found." << endl;
            return;
        }

        // 显示表头
        cout << "All records:" << endl;
        cout << "Date       | Type    | Amount     | Category        | Description" << endl;
        cout << "------------------------------------------------------------------------" << endl;

        // 遍历并显示所有记录
        for (const auto& record : records) {
            record.display();
        }

        // 显示当前余额
        cout << "\nCurrent balance: " << getBalance() << endl;
    }
};

// 主菜单 - 显示系统的主要功能选项
void displayMenu() {
    cout << "\n===== Personal Finance Management System =====" << endl;
    cout << "1. Add Income Record" << endl;           // 添加收入记录
    cout << "2. Add Expense Record" << endl;          // 添加支出记录
    cout << "3. View Income Records by Date Range" << endl;    // 按日期范围查看收入记录
    cout << "4. View Expense Records by Date Range" << endl;   // 按日期范围查看支出记录
    cout << "5. View Current Balance" << endl;        // 查看当前余额
    cout << "6. Modify Record by Date" << endl;       // 按日期修改记录
    cout << "7. Delete Record by Date" << endl;       // 按日期删除记录
    cout << "8. Category Statistics by Date Range" << endl;    // 按日期范围统计分类收支
    cout << "9. Show All Records" << endl;            // 显示所有记录
    cout << "10. Exit" << endl;                       // 退出系统
    cout << "Please select an option (1-10): ";
}

// 主函数 - 程序的入口点
int main() {
    FinanceManager manager;  // 创建财务管理器实例
    int choice;              // 用户选择的菜单选项

    // 主程序循环
    do {
        displayMenu();        // 显示主菜单
        cin >> choice;        // 获取用户输入

        // 根据用户选择执行相应功能
        switch (choice) {
        case 1:
            manager.addRecord("income");      // 添加收入记录
            break;
        case 2:
            manager.addRecord("expense");     // 添加支出记录
            break;
        case 3:
            manager.showRecordsByDateRange("income");   // 查看收入记录
            break;
        case 4:
            manager.showRecordsByDateRange("expense");  // 查看支出记录
            break;
        case 5:
            cout << "Current balance: " << manager.getBalance() << endl;  // 显示当前余额
            break;
        case 6:
            manager.modifyRecord();           // 修改记录
            break;
        case 7:
            manager.deleteRecord();           // 删除记录
            break;
        case 8:
            manager.categoryStatistics();     // 分类统计
            break;
        case 9:
            manager.showAllRecords();         // 显示所有记录
            break;
        case 10:
            cout << "Exiting system. Goodbye!" << endl;  // 退出系统
            break;
        default:
            cout << "Invalid option! Please try again." << endl;  // 无效选项提示
            break;
        }

        // 清除输入缓冲区，确保下次输入正常
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice != 10);  // 当用户选择退出时结束循环

    return 0;  // 程序正常结束
}