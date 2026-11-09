#include "FinanceManager.h"
#include "Transaction.h"
#include <iostream>
#include <algorithm>

// 构造函数实现
FinanceManager::FinanceManager(int target, FileHandler& handler)
    : financialTarget(target), fileHandler(handler), balance(0.0) {
    // 可以在这里调用加载记录的方法
    loadRecords();  // 启动时自动加载已有记录
}

// 加载记录（从文件读取到transactions向量）
void FinanceManager::loadRecords() {
    transactions = fileHandler.readTransactions();
    // 重新计算余额
    balance = 0.0;
    for (const auto& trans : transactions) {
        if (trans.get_Type() == "income") {
            balance += trans.get_Amount();
        }
        else {
            balance -= trans.get_Amount();
        }
    }
}

// 生成唯一ID
int FinanceManager::generateUniqueId() {
    return Transaction::nextId++;
}

// 添加收入（参数版本）
bool FinanceManager::addIncome(const std::string& date, double amount,
    const std::string& category, const std::string& description) {
    if (!Transaction::isValidDate(date) || amount <= 0) {
        return false;
    }
    Transaction income(date, "income", amount, category, description);
    transactions.push_back(income);
    balance += amount;
    return true;
}

// 添加支出（参数版本）
bool FinanceManager::addExpense(const std::string& date, double amount,
    const std::string& category, const std::string& description) {
    if (!Transaction::isValidDate(date) || amount <= 0) {
        return false;
    }
    Transaction expense(date, "expense", amount, category, description);
    transactions.push_back(expense);
    balance -= amount;
    return true;
}

// 添加收入（Transaction对象版本）
void FinanceManager::addIncome(const Transaction& income) {
    transactions.push_back(income);
    balance += income.get_Amount();
}

// 添加支出（Transaction对象版本）
void FinanceManager::addExpense(const Transaction& expense) {
    transactions.push_back(expense);
    balance -= expense.get_Amount();
}

// 更新记录
bool FinanceManager::updateRecord(int id, const std::string& newDate, const std::string& newType, double newAmount, const std::string& newCategory, const std::string& newDescription) {
    for (auto& trans : transactions) {
        if (trans.get_Id() == id) {
            // 1. 获取旧的金额和类型，以便能够正确地"撤销"它对收支平衡的影响
            double oldAmount = trans.get_Amount();
            std::string oldType = trans.get_Type();

            // 2. 根据旧的类型，"撤销"这笔旧交易对收支平衡v的影响
            if (oldType == "income") {
                balance -= oldAmount; // 如果旧的是收入，就减去旧金额
            }
            else { 
                balance += oldAmount; // 如果旧的是支出，就加上旧金额
            }

            // 3. 根据新的类型，"应用"这笔新交易对余额的影响
            if (newType == "income") {
                balance += newAmount; 
            }
            else { 
                balance -= newAmount; 
            }


            trans.set_Date(newDate);
            trans.set_Type(newType);
            trans.set_Amount(newAmount);
            trans.set_Category(newCategory);
            trans.set_Description(newDescription);
            return true;
        }
    }
    return false;
}

// 删除记录
bool FinanceManager::deleteRecord(int id) {
    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        if (it->get_Id() == id) {
            // 调整余额
            if (it->get_Type() == "income") {
                balance -= it->get_Amount();
            }
            else {
                balance += it->get_Amount();
            }
            transactions.erase(it);
            return true;
        }
    }
    return false;
}

// 计算当前余额
double FinanceManager::calculateBalance() {
    return balance;
}

// 检查余额提醒
std::string FinanceManager::checkBalanceReminder() {
    if (balance < 0) {
        return "警告：当前余额为负数，请注意控制支出！";
    }
    else if (financialTarget > 0 && balance >= financialTarget) {
        return "恭喜：已达成财务目标！";
    }
    else if (financialTarget > 0) {
        double remaining = financialTarget - balance;
        return "距离财务目标还剩 " + std::to_string(remaining) + " 元";
    }
    return "";
}

// 按日期查询记录
std::vector<Transaction> FinanceManager::queryRecordsByDate(const std::string& date) {
    std::vector<Transaction> result;
    for (const auto& trans : transactions) {
        if (trans.get_Date() == date) {
            result.push_back(trans);
        }
    }
    return result;
}

// 按日期范围查询收入
std::vector<Transaction> FinanceManager::queryIncomeByDate(const std::string& start, const std::string& end) {
    std::vector<Transaction> result;
    for (const auto& trans : transactions) {
        if (trans.get_Type() == "income" && trans.get_Date() >= start && trans.get_Date() <= end) {
            result.push_back(trans);
        }
    }
    return result;
}

// 按日期范围查询支出
std::vector<Transaction> FinanceManager::queryExpenseByDate(const std::string& start, const std::string& end) {
    std::vector<Transaction> result;
    for (const auto& trans : transactions) {
        if (trans.get_Type() == "expense" && trans.get_Date() >= start && trans.get_Date() <= end) {
            result.push_back(trans);
        }
    }
    return result;
}

// 按类别统计收入
std::map<std::string, double> FinanceManager::statisticIncomeByCategory(const std::string& start, const std::string& end) {
    std::map<std::string, double> result;
    for (const auto& trans : transactions) {
        if (trans.get_Type() == "income" && trans.get_Date() >= start && trans.get_Date() <= end) {
            result[trans.get_Category()] += trans.get_Amount();
        }
    }
    return result;
}

// 按类别统计支出
std::map<std::string, double> FinanceManager::statisticExpenseByCategory(const std::string& start, const std::string& end) {
    std::map<std::string, double> result;
    for (const auto& trans : transactions) {
        if (trans.get_Type() == "expense" && trans.get_Date() >= start && trans.get_Date() <= end) {
            result[trans.get_Category()] += trans.get_Amount();
        }
    }
    return result;
}

// 保存所有记录（实际项目中应实现文件存储逻辑）
bool FinanceManager::saveAllRecords() {
    try {
        // 调用FileHandler的写入方法
        bool success = fileHandler.writeTransactions(transactions);
        if (success) {
            std::cout << "数据保存成功，共保存 " << transactions.size() << " 条记录" << std::endl;
        }
        return success;
    }
    catch (const std::exception& e) {
        std::cout << "数据保存失败: " << e.what() << std::endl;
        return false;
    }
}
