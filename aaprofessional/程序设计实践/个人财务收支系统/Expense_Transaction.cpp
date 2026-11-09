#include "Expense_Transaction.h"
#include <iostream>

// 初始化
CategoryManager Expense_Transaction::categoryManager({ "餐饮","交通","购物","娱乐","医疗" }, "expense_categories.txt");


// *带id的构造函数实现（调用基类带id的构造函数）
Expense_Transaction::Expense_Transaction(int id, const std::string& date, double amount,
    const std::string& category, const std::string& description)
    : Transaction(id, date, "expense", amount, category, description) {
}


std::vector<std::string> Expense_Transaction::get_All_Expense_Categories() {
    return categoryManager.getAllCategories();
}

bool Expense_Transaction::is_Default_Expense_Categories(std::string category) {
    return categoryManager.isDefaultCategory(category);
}

bool Expense_Transaction::is_Custom_Expense_Categories(std::string category) {
    return categoryManager.isCustomCategory(category);
}

bool Expense_Transaction::add_Custom_Expense_Categories(std::string category) {
    return categoryManager.addCustomCategory(category);
}

bool Expense_Transaction::remove_Custom_Expense_Categories(std::string category) {
    return categoryManager.removeCustomCategory(category);
}

void Expense_Transaction::display_All_Expense_Categories() {
    categoryManager.displayAllCategories("支出");
}

int Expense_Transaction::get_Expense_Categories_Count() {
    return categoryManager.getCategoriesCount();
}

bool Expense_Transaction::save_Custom_Expense_Categories() {
    return categoryManager.saveCustomCategories();
}