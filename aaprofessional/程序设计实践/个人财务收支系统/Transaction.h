#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    int id;
    std::string date;
    std::string type;  // "income" 或 "expense"
    double amount;
    std::string category;
    std::string description;
    

public:
    static int nextId;  // 静态成员，用于生成唯一ID
    // 构造函数
    Transaction(int id, std::string date, std::string type, double amount,
        std::string category, std::string description);
    Transaction(std::string date, std::string type, double amount,
        std::string category, std::string description);

    // Get方法，由于获取数据
    int get_Id() const;
    std::string get_Date() const;
    std::string get_Type() const;
    double get_Amount() const;
    std::string get_Category() const;
    std::string get_Description() const;

    // Set方法，用于修改记录
    void set_Date(std::string newDate);
    void set_Type(std::string newType);
    void set_Amount(double newAmount);
    void set_Category(std::string newCategory);
    void set_Description(std::string newDescription);

    
    void display();//打印记录条
    std::string Format_Conversion() const;  // 格式转换，用于文件存储
    static bool isValidDate(const std::string& date);//基层日期验证
};

#endif // TRANSACTION_H
