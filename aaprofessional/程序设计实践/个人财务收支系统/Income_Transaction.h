#ifndef INCOME_TRANSACTION_H
#define INCOME_TRANSACTION_H

#include "Transaction.h"
#include "CategoryManager.h"
#include <vector>
#include <string>

class Income_Transaction : public Transaction {
private:
    static CategoryManager categoryManager;//通过实例来转发请求

public:
    // 带id的构造函数声明
    Income_Transaction(int id, const std::string& date, double amount,
        const std::string& category, const std::string& description);

    // 获取所有类别
    static std::vector<std::string> get_All_Income_Categories();
    //添加、删除
    static bool add_Custom_Income_Categories(std::string category);
    static bool remove_Custom_Income_Categories(std::string category);
    //判断是否存在
    static bool is_Default_Income_Categories(std::string category);
    static bool is_Custom_Income_Categories(std::string category);
    
    static void display_All_Income_Categories();//打印
    static int get_Income_Categories_Count();//已有类别个数
    static bool save_Custom_Income_Categories();//保存为自定义类别

};

#endif // INCOME_TRANSACTION_H