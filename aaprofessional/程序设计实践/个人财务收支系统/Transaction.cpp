#include "Transaction.h"
#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <iomanip>

// 初始化静态成员
int Transaction::nextId = 1;

// 6参数构造函数实现
Transaction::Transaction(int id, std::string date, std::string type, double amount,
    std::string category, std::string description)
    : id(id), date(date), type(type), amount(amount),
    category(category), description(description) {
    if (id >= nextId) {
        nextId = id + 1;
    }
}

// 5参数构造函数实现
Transaction::Transaction(std::string date, std::string type, double amount,
    std::string category, std::string description)
    : date(date), type(type), amount(amount), category(category),
    description(description), id(nextId++) {
}

// Getter实现
int Transaction::get_Id() const { return id; }
std::string Transaction::get_Date() const { return date; }
std::string Transaction::get_Type() const { return type; }
double Transaction::get_Amount() const { return amount; }
std::string Transaction::get_Category() const { return category; }
std::string Transaction::get_Description() const { return description; }

// Setter实现
void Transaction::set_Date(std::string newDate) { 
    date = newDate;
}
void Transaction::set_Type(std::string newType) { 
    type = newType; 
}
void Transaction::set_Amount(double newAmount) { 
    amount = newAmount; 
}
void Transaction::set_Category(std::string newCategory) { 
    category = newCategory; 
}
void Transaction::set_Description(std::string newDescription) { 
    description = newDescription; 
}

// 显示交易信息
void Transaction::display() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << date << " | " << type << " | " << amount << " | "
        << category << " | " << description;
    std::cout << ss.str() << std::endl;
}

// 格式转换（用于文件存储）
std::string Transaction::Format_Conversion() const {  // 添加const修饰符
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << id << "," << date << "," << type << "," << amount << ","
        << category << "," << description;
    return ss.str();
}

// 日期验证
bool Transaction::isValidDate(const std::string& date) {
    if (date.length() != 8) return false;

    for (char c : date) {
        if (!isdigit(c)) return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(4, 2));
    int day = stoi(date.substr(6, 2));

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    const int daysInMonth[] = { 31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31 };
    if (day > daysInMonth[month - 1]) return false;

    return true;
}
