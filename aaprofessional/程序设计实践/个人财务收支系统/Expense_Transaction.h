#ifndef EXPENSE_TRANSACTION_H
#define EXPENSE_TRANSACTION_H

#include "Transaction.h"
#include "CategoryManager.h"
#include <vector>
#include <string>

class Expense_Transaction : public Transaction {
private:
    static CategoryManager categoryManager;

public:
    // *带id的构造函数声明
    Expense_Transaction(int id, const std::string& date, double amount,
        const std::string& category, const std::string& description);

    static std::vector<std::string> get_All_Expense_Categories();
    static bool add_Custom_Expense_Categories(std::string category);
    static bool remove_Custom_Expense_Categories(std::string category);
    static bool is_Default_Expense_Categories(std::string category);
    static bool is_Custom_Expense_Categories(std::string category);
    static void display_All_Expense_Categories();
    static int get_Expense_Categories_Count();
    static bool save_Custom_Expense_Categories();
};

#endif // EXPENSE_TRANSACTION_H