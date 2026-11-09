#ifndef USERINTERFACE_H
#define USERINTERFACE_H  // 防止头文件重复包含（预处理指令）

// 包含必要的标准库（存储数据的容器和字符串）
#include <vector>
#include <map>
#include <string>
#include <utility>



class FinanceManager;
class Transaction;

class UserInterface {
private:
    // 私有成员（仅类内部使用）
    FinanceManager* financeManager;  // 指向业务核心类的指针
    //通过这个指针调用FinanceManager的方法
    int selectedOption = -1;  // 记录用户当前选择的菜单选项
    std::string getCurrentDate();

public:
    // 构造函数（必须有）
    UserInterface(FinanceManager* fm);  // 传入FinanceManager指针，初始化时绑定业务层

    // 1. 菜单控制相关方法（用户交互入口）
    void showMainMenu();                 // 显示主菜单（打印选项列表）
    int getValidMenuOption();            // 获取并验证用户输入的菜单选项（确保是0-9的数字）
    bool handleMenuOption(int option);   // 根据用户选择的选项，跳转对应功能

    // 2. 收支添加流程（核心交互功能）
    void addIncomeFlow();                // 添加收入的完整交互流程
    void addExpenseFlow();               // 添加支出的完整交互流程

    // 3. 结果展示相关（将数据以用户易懂的形式输出）
    void showRecords(const std::vector<Transaction>& records);  // 展示交易记录列表
    void showStatistic(const std::map<std::string, double>& result);  // 展示类别统计结果
    void showBalanceReminder(const std::string& reminder);      // 展示余额提醒信息

    // 4. 退出系统
    void exitSystem();                   // 处理退出逻辑（保存数据→提示→终止程序）

private:
    // 私有辅助方法（仅内部调用，简化重复逻辑）
    std::pair<std::string, std::string> getDateRange();  // 获取日期范围（开始+结束），用于查询/统计
    std::string getSingleDate();                         // 获取单个日期，用于查找/修改/删除记录
    void manageIncomeCategoriesFlow();
    void manageExpenseCategoriesFlow();
};

#endif // USERINTERFACE_H