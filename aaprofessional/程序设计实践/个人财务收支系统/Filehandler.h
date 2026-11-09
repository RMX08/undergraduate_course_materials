#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include "Transaction.h"

class FileHandler {
private:
    std::string filepath;
    std::string targetFilepath;  // *财务目标文件路径

public:
    // 构造函数
    FileHandler(const std::string& transactionsFile, 
               const std::string& targetFile = "financial_target.txt");

    // 交易记录相关方法
    std::vector<Transaction> readTransactions();
    bool writeTransactions(const std::vector<Transaction>& transactions);

    // *财务目标相关方法
    int readFinancialTarget();
    bool saveFinancialTarget(int target);

    // 异常处理
    void handleFileExceptions(const std::string& errorMsg);
};

#endif // FILEHANDLER_H
    