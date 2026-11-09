#include "UserInterface.h"
#include "FinanceManager.h"
#include "Transaction.h"
#include "Income_Transaction.h"
#include "Expense_Transaction.h"
#include <ctime>
#include <chrono> 
#define byte BYTE  
#include <windows.h>  
#include <iostream>
#include <sstream>
#include <iomanip> 
#include <algorithm>


using namespace std;

UserInterface::UserInterface(FinanceManager* fm) {
    this->financeManager = fm;
}

string UserInterface::getCurrentDate() {
    // 获取当前系统时间
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    // 使用安全版本的 localtime_s
    localtime_s(&local_time, &now_time);

    // 格式化为YYYYMMDD字符串
    stringstream ss;
    ss << setw(4) << std::setfill('0') << (local_time.tm_year + 1900)
        << setw(2) << std::setfill('0') << (local_time.tm_mon + 1)
        << setw(2) << std::setfill('0') << local_time.tm_mday;
    return ss.str();
}

void UserInterface::showMainMenu() {
    system("cls");
    cout << "==================== 个人财务收支管理系统 ====================\n";
    cout << "1  - 添加收入记录\t\t2  - 添加支出记录\n";
    cout << "3  - 查看日期内收入记录\t\t4  - 查看日期内支出记录\n";
    cout << "5  - 查看实时收支平衡\t\t6  - 按日期查找记录\n";
    cout << "7  - 按日期修改记录\t\t8  - 按日期删除记录\n";
    cout << "9  - 统计日期内类别收支\t\t10 - 管理收入类别\n";
    cout << "11 - 管理支出类别\t\t0  - 退出系统\n";
    cout << "=============================================================\n";
}

int UserInterface::getValidMenuOption() {
    int option;
    while (true) {
        cout << "请输入操作编号（0-11）：";
        cin >> option;

        if (cin.fail() || option < 0 || option > 11) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效！请重新输入0-11之间的数字。\n";
        }
        else {
            cin.ignore(1000, '\n');
            return option;
        }
    }
}

// 辅助方法：获取日期范围（开始日期+结束日期）
pair<string, string> UserInterface::getDateRange() {
    string startDate, endDate;
    string currentDate = getCurrentDate(); // 先获取今天的日期（YYYYMMDD）

    // 1. 单独处理 start 日期：格式正确 + 不早于今天 + 不晚于今天
    do {
        cout << "请输入开始日期（格式YYYYMMDD）：";
        cin >> startDate;

        // 第一重校验：格式是否正确（原逻辑保留）
        if (!Transaction::isValidDate(startDate)) {
            cout << "开始日期格式无效！请重新输入：\n";
        }
        // 第二重校验：是否超过当前日期（新增！输入时即时判断）
        else if (startDate > currentDate) {
            cout << "开始日期不能晚于今天（" << currentDate << "）！请重新输入：\n";
        }
        // 两重校验都通过，退出 start 日期循环
        else {
            break;
        }
    } while (true);

    // 2. 处理 end 日期：格式正确 + 不早于 start + 不晚于今天 + 错了重新输整个范围
    do {
        cout << "请输入结束日期（格式YYYYMMDD）：";
        cin >> endDate;

        // 校验1：格式是否正确
        if (!Transaction::isValidDate(endDate)) {
            cout << "结束日期格式无效！将重新输入整个日期范围...\n";
            // 重新调用自身，从头开始输入（包括 start 日期）
            return getDateRange();
        }
        // 校验2：是否晚于今天
        else if (endDate > currentDate) {
            cout << "结束日期不能晚于今天（" << currentDate << "）！将重新输入整个日期范围...\n";
            return getDateRange();
        }
        // 校验3：是否早于 start 日期
        else if (endDate < startDate) {
            cout << "结束日期不能早于开始日期！将重新输入整个日期范围...\n";
            return getDateRange();
        }
        // 所有校验通过，退出 end 日期循环
        else {
            break;
        }
    } while (true);

    return { startDate, endDate };
}


// 辅助方法：获取单个日期
string UserInterface::getSingleDate() {
    string date;
    string currentDate = getCurrentDate();

    do {
        cout << "请输入日期（格式YYYYMMDD）：";
        cin >> date;

        if (!Transaction::isValidDate(date)) {
            cout << "日期格式无效！请重新输入：\n";
        }
        else if (date > currentDate) { // 新增：即时判断是否超今天
            cout << "日期不能晚于今天（" << currentDate << "）！请重新输入：\n";
        }
        else {
            break;
        }
    } while (true);

    return date;
}

bool UserInterface::handleMenuOption(int option) {
    switch (option) {
    case 1:
        addIncomeFlow();
        break;

    case 2:
        addExpenseFlow();
        break;

    case 3: {
        auto [start, end] = getDateRange();
        vector<Transaction> incomes = financeManager->queryIncomeByDate(start, end);
        showRecords(incomes);
        system("pause");
        break;
    }

    case 4: {
        auto [start, end] = getDateRange();
        vector<Transaction> expenses = financeManager->queryExpenseByDate(start, end);
        showRecords(expenses);
        system("pause");
        break;
    }

    case 5: {
        double balance = financeManager->calculateBalance();
        string reminder = financeManager->checkBalanceReminder();
        cout << "当前实时余额：" << fixed << setprecision(2) << balance << " 元\n";
        if (!reminder.empty()) showBalanceReminder(reminder);
        system("pause");
        break;
    }

    case 6: {
        string date = getSingleDate();
        vector<Transaction> records = financeManager->queryRecordsByDate(date);
        showRecords(records);
        system("pause");
        break;
    }


    case 7: {
        string date = getSingleDate();
        vector<Transaction> records = financeManager->queryRecordsByDate(date);
        if (records.empty()) {
            cout << "该日期无记录可修改\n";
            system("pause");
            break;
        }

        showRecords(records);
        int targetId;
        cout << "请输入要修改的记录id：";
        cin >> targetId;

        // 修正：添加显式类型转换，解决C4267警告
        int index = -1;
        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].get_Id() == targetId) {
                index = static_cast<int>(i);  // 显式转换
                break;
            }
        }
        if (index == -1) {
            cout << "无效的id\n";
            system("pause");
            break;
        }

        // 将选中记录的当前值存储到临时变量中，直到最后才一次性保存
        Transaction& originalRecord = records[index]; // 获取原始记录的引用
        string newDate = originalRecord.get_Date();
        string newType = originalRecord.get_Type();
        double newAmount = originalRecord.get_Amount();
        string newCategory = originalRecord.get_Category();
        string newDesc = originalRecord.get_Description();

        // 循环，让用户选择修改哪个字段
        while (true) 
        {
            system("cls");
            cout << "==================== 正在修改记录 ID: " << targetId << " ====================\n";
            cout << "1 - 日期    : " << newDate << "\n";
            cout << "2 - 类型    : " << (newType == "income" ? "收入" : "支出") << "\n";
            cout << "3 - 金额    : " << fixed << setprecision(2) << newAmount << "\n";
            cout << "4 - 类别    : " << newCategory << "\n";
            cout << "5 - 备注    : " << newDesc << "\n";
            cout << "--------------------------------------------------------\n";
            cout << "0 - 完成修改并保存\n";
            cout << "========================================================\n";
            cout << "请输入要修改的字段编号 (0-5): ";

            int choice;
            cin >> choice;
            cin.ignore(1000, '\n'); 

            // 输入0，表示所有字段都修改完毕，跳出编辑循环
            if (choice == 0) 
                break;

            switch (choice) 
            {
                case 1:  // 修改日期
                {
                    cout << "请输入新日期 (格式 YYYYMMDD): ";
                    string tempDate;
                    while(cin >> tempDate)
                    {
                        cin.clear(); 
                        cin.ignore(1000, '\n');
                        if (Transaction::isValidDate(tempDate)) 
                        {
                            newDate = tempDate;
                            cout << "修改成功！\n";
                            system("pause");
                            break;
                        }
                        else
                        {
                            cout << "日期格式无效！请重新输入: \n";
                            system("pause");
                        }
                    }
                    break;
                }
                case 2: // 修改类型
                { 
                    cout << "请输入新类型（收入/支出）: ";
                    string typeChoice;
                    while(cin >> typeChoice)
                    {
                        cin.clear(); 
                        cin.ignore(1000, '\n');
                        if (typeChoice == "收入") 
                        {
                            newType = "income";
                            cout << "修改成功！\n";
                            system("pause");
                            break;
                        }
                        else if (typeChoice == "支出") 
                        {
                            newType = "expense";
                            cout << "修改成功！\n";
                            system("pause");
                            break;
                        }
                        else 
                        {
                            cout << "无效的选择！请重新输入：\n";
                            system("pause");
                        }
                    }
                    break;
                }
                case 3: // 修改金额
                { 
                    cout << "请输入新金额 (必须是正数): ";
                    double tempAmount;
                    while (cin >> tempAmount)
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');

                        if (tempAmount > 0) 
                        { 
                            newAmount = tempAmount;
                            cout << "修改成功！\n";
                            system("pause");
                            break;
                        } 
                        else 
                        {
                            cout << "金额必须为正数！请重新输入：\n";
                            system("pause");
                        }
                    }
                    
                    break;
                }
                case 4: // 修改类别
                { 
                    if (newType == "income")
                        Income_Transaction::display_All_Income_Categories();
                    else
                        Expense_Transaction::display_All_Expense_Categories();

                    while (true) 
                    {
                        cout << "请输入新类别: ";
                        string tempCategory;
                        getline(cin, tempCategory); 

                        bool isCategoryValid = false; // 用于判断输入是否有效

                        
                        if (newType == "income") 
                        {
                            vector<string> allIncomeCategories = Income_Transaction::get_All_Income_Categories();

                            // 在列表中查找用户输入的类别
                            if (std::find(allIncomeCategories.begin(), allIncomeCategories.end(), tempCategory) != allIncomeCategories.end()) {
                                isCategoryValid = true; 
                            }
                        } 
                        else 
                        {
                            vector<string> allExpenseCategories = Expense_Transaction::get_All_Expense_Categories();
                            
                            // 在支出类别列表中查找
                            if (std::find(allExpenseCategories.begin(), allExpenseCategories.end(), tempCategory) != allExpenseCategories.end()) {
                                isCategoryValid = true; 
                            }
                        }

                        if (isCategoryValid)
                        {
                            newCategory = tempCategory; 
                            cout << "修改成功！\n";
                            system("pause");
                            break; 
                        } 
                        else 
                        {
                            cout << "\n输入的类别不存在！请从已有列表中选择一个输入。\n";
                            cout << "当前可用的 \"" << (newType == "income" ? "收入" : "支出") << "\" 类别有：\n";
                            
                            if (newType == "income") {
                                Income_Transaction::display_All_Income_Categories();
                            } else {
                                Expense_Transaction::display_All_Expense_Categories();
                            }
                            cout << "----------------------------------------------------\n";
                        }
                    }
                    break;
                }
                case 5: { // 修改备注
                    cout << "请输入新备注: ";
                    getline(cin, newDesc);
                    cout << "修改成功！\n";
                    system("pause");
                    break;
                }
                default:
                    cout << "无效的编号！\n";
                    system("pause");
                    break;
            }
        }

        bool success = financeManager->updateRecord(targetId, newDate, newType, newAmount, newCategory, newDesc);
        if (success) cout << "修改成功\n";
        else cout << "修改失败\n";
        system("pause");
        break;
    }

    case 8: {
        string date = getSingleDate();
        vector<Transaction> records = financeManager->queryRecordsByDate(date);
        if (records.empty()) {
            cout << "该日期无记录可删除\n";
            system("pause");
            break;
        }

        showRecords(records);
        int targetId;
        cout << "请输入要删除的记录id：";
        cin >> targetId;

        int index = -1;
        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].get_Id() == targetId) {
                index = static_cast<int>(i);  // 显式转换
                break;
            }
        }
        if (index == -1) {
            cout << "无效的id\n";
            system("pause");
            break;
        }

        bool success = financeManager->deleteRecord(targetId);
        if (success) cout << "删除成功\n";
        else cout << "删除失败\n";
        system("pause");
        break;
    }

    case 9: {
        auto [start, end] = getDateRange();
        cout << "1 - 统计收入类别  2 - 统计支出类别：";
        int type;
        cin >> type;
        map<string, double> result;
        if (type == 1) {
            result = financeManager->statisticIncomeByCategory(start, end);
        }
        else if (type == 2) {
            result = financeManager->statisticExpenseByCategory(start, end);
        }
        else {
            cout << "选择无效！\n";
            system("pause");
            break;
        }
        showStatistic(result);
        system("pause");
        break;
    }

    case 10:
        manageIncomeCategoriesFlow();
        break;

    case 11:
        manageExpenseCategoriesFlow();
        break;

    case 0:
        exitSystem();
        break;

    default:
        cout << "功能待实现！按任意键返回...\n";
        system("pause");
        break;
    }
    return true;
}

// 以下为原有方法实现（保持不变）
void UserInterface::addIncomeFlow() {
    system("cls");
    cout << "==================== 添加收入记录 ====================\n";

    string date, category, desc;
    double amount;

    cout << "请输入日期（格式YYYYMMDD）：";
    cin >> date;
    while (!Transaction::isValidDate(date)) {
        cout << "日期无效！请重新输入：";
        cin >> date;
    }

    cout << "请输入收入金额（正数）：";  // 修正：原代码写成了"支出金额"
    while (!(cin >> amount) || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "金额无效！请输入正数：";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Income_Transaction::display_All_Income_Categories();

    while (true){
        cout << "请输入收入类别（如工资/兼职）：";
        
        getline(cin, category);
        bool isCategoryValid = false; 

        vector<string> incomeCategories = Income_Transaction::get_All_Income_Categories();
        if (find(incomeCategories.begin(), incomeCategories.end(), category) != incomeCategories.end()) {
            isCategoryValid = true; // 找到了，输入有效
        }

        if (isCategoryValid) break;
        else {
            // 验证失败
            cout << "\n 输入的类别不存在！请重新输入。\n";
        }
    }
    

    cout << "请输入备注（可选）：";
    getline(cin, desc);

    if (financeManager->addIncome(date, amount, category, desc)) {
        cout << "收入记录添加成功！\n";
    }
    else {
        cout << "收入记录添加失败！\n";
    }
    system("pause");
}

void UserInterface::addExpenseFlow() {
    system("cls");
    cout << "==================== 添加支出记录 ====================\n";

    string date, category, desc;
    double amount;

    cout << "请输入日期（格式YYYYMMDD）：";
    cin >> date;
    while (!Transaction::isValidDate(date)) {
        cout << "日期无效！请重新输入：";
        cin >> date;
    }

    cout << "请输入支出金额（正数）：";
    while (!(cin >> amount) || amount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "金额无效！请输入正数：";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Expense_Transaction::display_All_Expense_Categories();

    while (true){
        cout << "请输入支出类别（如餐饮/交通）：";
        getline(cin, category);
        bool isCategoryValid = false; 

        vector<string> expenseCategories = Expense_Transaction::get_All_Expense_Categories();
        if (find(expenseCategories.begin(), expenseCategories.end(), category) != expenseCategories.end()) {
            isCategoryValid = true; // 找到了，输入有效
        }

        if (isCategoryValid) break;
        else {
            // 验证失败
            cout << "\n 输入的类别不存在！请重新输入。\n";
        }
    }

    cout << "请输入备注（可选）：";
    getline(cin, desc);

    if (financeManager->addExpense(date, amount, category, desc)) {
        cout << "支出记录添加成功！\n";
        string reminder = financeManager->checkBalanceReminder();
        if (!reminder.empty()) showBalanceReminder(reminder);
    }
    else {
        cout << "支出记录添加失败！\n";
    }
    system("pause");
}

void UserInterface::showRecords(const vector<Transaction>& records) {
    system("cls");
    cout << "==================== 记录列表 ====================\n";
    if (records.empty()) {
        cout << "无任何记录\n";
        return;
    }

    cout << "id  | 日期       | 类型   | 金额(元) | 类别   | 备注\n";
    cout << "-----------------------------------------------------\n";
    for (const auto& record : records) {  // 修正：使用范围for循环避免索引转换
        cout << left << setw(4) << record.get_Id()
            << "| " << setw(11) << record.get_Date()
            << "| " << setw(7) << (record.get_Type() == "income" ? "收入" : "支出")
            << "| " << setw(9) << fixed << setprecision(2) << record.get_Amount()
            << "| " << setw(7) << record.get_Category()
            << "| " << record.get_Description() << "\n";
    }
}

void UserInterface::showStatistic(const map<string, double>& result) {
    system("cls");
    cout << "==================== 类别收支统计 ====================\n";
    if (result.empty()) {
        cout << "无统计数据！\n";
        return;
    }

    double total = 0;
    for (const auto& pair : result) {
        cout << pair.first << "：" << fixed << setprecision(2) << pair.second << " 元\n";
        total += pair.second;
    }
    cout << "-----------------------------------------------------\n";
    cout << "合计：" << fixed << setprecision(2) << total << " 元\n";
}

void UserInterface::showBalanceReminder(const string& reminder) {
    cout << "\n==================== 余额提醒 ====================\n";
    cout << reminder << "\n";
    cout << "===================================================\n";
}

void UserInterface::exitSystem() {
    cout << "\n正在保存数据...\n";
    //新增：保存自定义类别
    bool incomeCatSaved = Income_Transaction::save_Custom_Income_Categories();
    bool expenseCatSaved = Expense_Transaction::save_Custom_Expense_Categories();

    if (incomeCatSaved && expenseCatSaved) {
        cout << "自定义类别保存成功！\n";
    }
    else {
        cout << "自定义类别保存失败！请检查文件权限。\n";
    }
    Sleep(700);
    if (financeManager->saveAllRecords()) {
        cout << "数据保存成功！\n";
    }
    else {
        cout << " 数据保存失败，请检查文件权限！\n";
    }
    Sleep(700);
    cout << "感谢使用，再见！\n";
    system("pause");
    exit(0);
}

void UserInterface::manageIncomeCategoriesFlow() 
{
    // 使用循环，让用户可以执行多次操作（添加/删除）而无需退回主菜单
    while (true) {
        system("cls");
        cout << "==================== 管理收入类别 ====================\n";

        // 第一步：输出所有当前收入类别
        Income_Transaction::display_All_Income_Categories();
        cout << "----------------------------------------------------\n";
        cout << "1 - 添加新的自定义类别\n";
        cout << "2 - 删除一个自定义类别\n";
        cout << "0 - 返回主菜单\n";
        cout << "====================================================\n";

        int choice;
        cout << "请输入操作编号 (0-2): ";
        cin >> choice;

        // 输入验证
        if (cin.fail() || choice < 0 || choice > 2) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效！请重新输入。\n";
            system("pause");
            continue; // 继续下一次循环，重新显示菜单
        }
        cin.ignore(1000, '\n'); // 清除输入缓冲区中的换行符

        // 根据用户选择执行操作
        if (choice == 0) {
            break; // 用户选择0，跳出循环，返回主菜单
        }

        switch (choice) {
            case 1: { // 添加类别
                string newCategory;
                char choice1;
                cout << "请输入要添加的新类别名称: ";

                while(true)
                {
                    getline(cin,newCategory);
                    cout << "请确认输入的新类别名称是否正确 -  '" << newCategory << "' (Y/N): ";
                    while (!(cin >> choice1) || (choice1 != 'Y' && choice1 != 'y' && choice1 != 'N' && choice1 != 'n'))
                    {
                        cin.clear(); 
                        cin.ignore(1000, '\n'); 
                        cout << "输入无效！请输入正确值（Y/N）：";
                    }
                    cin.ignore(1000, '\n');

                    if (choice1 == 'Y' || choice1 == 'y')
                        break;
                    else if (choice1 == 'N' || choice1 == 'n')
                        cout << "请重新输入类别名称： ";
            
                }

                if (Income_Transaction::add_Custom_Income_Categories(newCategory)) {
                    cout << "\n类别 '" << newCategory << "' 添加成功！\n";
                }
                else {
                    cout << "\n添加失败！该类别可能已存在。\n";
                }
                break;
            }
            case 2: { // 删除类别
                string categoryToDelete;
                cout << "请输入要删除的自定义类别名称: ";
                getline(cin, categoryToDelete);

                if (Income_Transaction::remove_Custom_Income_Categories(categoryToDelete)) {
                    cout << "\n类别 '" << categoryToDelete << "' 删除成功！\n";
                }
                else {
                    cout << "\n删除失败！请确保该类别是自定义类别且存在。\n";
                    cout << "注意：默认类别无法删除。\n";
                }
                break;
            }
        }
        cout << "按任意键继续...";
        system("pause > nul"); // 暂停，等待用户按键，并隐藏系统提示
    }
}


void UserInterface::manageExpenseCategoriesFlow() {
    // 使用循环，让用户可以执行多次操作（添加/删除）而无需退回主菜单
    while (true) {
        system("cls");
        cout << "==================== 管理支出类别 ====================\n";

        // 第一步：输出所有当前收入类别
        Expense_Transaction::display_All_Expense_Categories();
        cout << "----------------------------------------------------\n";
        cout << "1 - 添加新的自定义类别\n";
        cout << "2 - 删除一个自定义类别\n";
        cout << "0 - 返回主菜单\n";
        cout << "====================================================\n";

        int choice;
        cout << "请输入操作编号 (0-2): ";
        cin >> choice;

        // 输入验证
        if (cin.fail() || choice < 0 || choice > 2) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效！请重新输入。\n";
            system("pause");
            continue; // 继续下一次循环，重新显示菜单
        }
        cin.ignore(1000, '\n'); // 清除输入缓冲区中的换行符

        // 根据用户选择执行操作
        if (choice == 0) {
            break; // 用户选择0，跳出循环，返回主菜单
        }

        switch (choice) {
            case 1: { // 添加类别
                string newCategory;
                char choice1;
                cout << "请输入要添加的新类别名称: ";

                while(true)
                {
                    getline(cin,newCategory);
                    cout << "请确认输入的新类别名称是否正确 -  '" << newCategory << "' (Y/N): ";
                    while (!(cin >> choice1) || (choice1 != 'Y' && choice1 != 'y' && choice1 != 'N' && choice1 != 'n'))
                    {
                        cin.clear(); 
                        cin.ignore(1000, '\n'); 
                        cout << "输入无效！请输入正确值（Y/N）：";
                    }
                    cin.ignore(1000, '\n');

                    if (choice1 == 'Y' || choice1 == 'y')
                        break;
                    else if (choice1 == 'N' || choice1 == 'n')
                        cout << "请重新输入类别名称： ";
            
                }

                if (Expense_Transaction::add_Custom_Expense_Categories(newCategory)) {
                    cout << "\n类别 '" << newCategory << "' 添加成功！\n";
                }
                else {
                    cout << "\n添加失败！该类别可能已存在。\n";
                }
                break;
            }
            case 2: { // 删除类别
                string categoryToDelete;
                cout << "请输入要删除的自定义类别名称: ";
                getline(cin, categoryToDelete);

                if (Expense_Transaction::remove_Custom_Expense_Categories(categoryToDelete)) {
                    cout << "\n类别 '" << categoryToDelete << "' 删除成功！\n";
                }
                else {
                    cout << "\n删除失败！请确保该类别是自定义类别且存在。\n";
                    cout << "注意：默认类别无法删除。\n";
                }
                break;
            }
        }
        cout << "按任意键继续...";
        system("pause > nul"); // 暂停，等待用户按键，并隐藏系统提示
    }
}