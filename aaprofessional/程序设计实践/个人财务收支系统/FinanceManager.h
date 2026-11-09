#ifndef FINANCEMANAGER_H
#define FINANCEMANAGER_H

#include <vector>
#include <string>
#include <map>
#include "Transaction.h"
#include "FileHandler.h"  // 包含文件处理器头文件

class FinanceManager {
private:
    std::vector<Transaction> transactions;
    double balance;

    int financialTarget;
    FileHandler& fileHandler;  // 持有FileHandler引用
    int generateUniqueId();

public:
    // 构造函数：接收财务目标和FileHandler引用
    FinanceManager(int target, FileHandler& handler);

    // 加载记录（新增方法）
    void loadRecords();

    // 原有方法声明
    bool addIncome(const std::string& date, double amount,
        const std::string& category, const std::string& description);
    bool addExpense(const std::string& date, double amount,
        const std::string& category, const std::string& description);
    void addIncome(const Transaction& income);
    void addExpense(const Transaction& expense);
    bool updateRecord(int id, const std::string& newDate, const std::string& newType, double newAmount, const std::string& newCategory, const std::string& newDescription);
    bool deleteRecord(int id);
    double calculateBalance();
    std::string checkBalanceReminder();
    bool saveAllRecords();  
    std::vector<Transaction> queryRecordsByDate(const std::string& date);
    std::vector<Transaction> queryIncomeByDate(const std::string& start, const std::string& end);
    std::vector<Transaction> queryExpenseByDate(const std::string& start, const std::string& end);
    std::map<std::string, double> statisticIncomeByCategory(const std::string& start, const std::string& end);
    std::map<std::string, double> statisticExpenseByCategory(const std::string& start, const std::string& end);
};

#endif // FINANCEMANAGER_H
