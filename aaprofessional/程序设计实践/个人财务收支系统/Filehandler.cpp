#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// 构造函数实现
FileHandler::FileHandler(const std::string& transactionsFile, const std::string& targetFile)
    : filepath(transactionsFile), targetFilepath(targetFile) {
}

// 实现异常处理函数
void FileHandler::handleFileExceptions(const std::string& errorMsg) {
    std::cerr << "文件操作错误: " << errorMsg << std::endl;
    throw std::runtime_error(errorMsg);
}

// 读取交易记录
std::vector<Transaction> FileHandler::readTransactions() {
    std::vector<Transaction> transactions;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        // 如果是首次运行，文件可能不存在，不抛出异常
        std::cerr << "提示: 交易记录文件不存在，将创建新文件。\n";
        return transactions;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int id;
        std::string date, type, category, description;
        double amount;
        char comma;

        // 读取ID和其他字段
        if (!(ss >> id >> comma) || comma != ',') {
            handleFileExceptions("记录格式错误: " + line);
            continue;
        }

        std::getline(ss, date, ',');
        std::getline(ss, type, ',');

        if (!(ss >> amount >> comma) || comma != ',') {
            handleFileExceptions("金额格式错误: " + line);
            continue;
        }

        std::getline(ss, category, ',');
        std::getline(ss, description);

        transactions.emplace_back(id, date, type, amount, category, description);
    }

    file.close();
    return transactions;
}

// 写入交易记录
bool FileHandler::writeTransactions(const std::vector<Transaction>& transactions) {
    std::ofstream file(filepath);  // 打开文件（若不存在则创建）

    if (!file.is_open()) {
        handleFileExceptions("无法写入文件: " + filepath);
        return false;
    }

    // 直接写入Format_Conversion的结果（已包含ID）
    for (const auto& t : transactions) {
        file << t.Format_Conversion() << "\n";  // 移除重复的t.get_Id()
    }

    file.close();
    return true;
}

// 新增：读取财务目标
int FileHandler::readFinancialTarget() {
    std::ifstream file(targetFilepath);
    int target = -1;

    if (!file.is_open()) {
        // 目标文件不存在，返回-1表示需要创建
        return -1;
    }

    if (file >> target && target > 0) {
        file.close();
        return target;
    }

    file.close();
    return -1; // 目标文件格式错误
}

// 新增：保存财务目标
bool FileHandler::saveFinancialTarget(int target) {
    if (target <= 0) {
        handleFileExceptions("无效的财务目标金额");
        return false;
    }

    std::ofstream file(targetFilepath);
    if (!file.is_open()) {
        handleFileExceptions("无法写入财务目标文件: " + targetFilepath);
        return false;
    }

    file << target;
    file.close();
    return true;
}
