#include "Income_Transaction.h"
#include <iostream>

// 初始化收入类别管理器（保持不变）
CategoryManager Income_Transaction::categoryManager({ "工资","奖金","零花钱","投资收益" }, "income_categories.txt");


// 新增：带id的构造函数实现（调用基类带id的构造函数）
Income_Transaction::Income_Transaction(int id, const std::string& date, double amount,
    const std::string& category, const std::string& description)
    : Transaction(id, date, "income", amount, category, description) {
}

// 原有方法实现保持不变
std::vector<std::string> Income_Transaction::get_All_Income_Categories() {
    return categoryManager.getAllCategories();
}

bool Income_Transaction::is_Default_Income_Categories(std::string category) {
    return categoryManager.isDefaultCategory(category);
}

bool Income_Transaction::is_Custom_Income_Categories(std::string category) {
    return categoryManager.isCustomCategory(category);
}

bool Income_Transaction::add_Custom_Income_Categories(std::string category) {
    return categoryManager.addCustomCategory(category);
}

bool Income_Transaction::remove_Custom_Income_Categories(std::string category) {
    return categoryManager.removeCustomCategory(category);
}

void Income_Transaction::display_All_Income_Categories() {
    categoryManager.displayAllCategories("收入");
}

int Income_Transaction::get_Income_Categories_Count() {
    return categoryManager.getCategoriesCount();
}

bool Income_Transaction::save_Custom_Income_Categories() {
    return categoryManager.saveCustomCategories();
}