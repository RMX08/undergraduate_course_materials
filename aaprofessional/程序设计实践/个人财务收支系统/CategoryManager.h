#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

class CategoryManager {
protected:
    unordered_set<string> default_categories;//无序的容器，自动防止重复添加
    unordered_set<string> custom_categories;

public:
    CategoryManager(const vector<string>& default_cats, const string& filepath);

    
    vector<string> getAllCategories() const;// 获取所有类别
    void displayAllCategories(const string& type) const;// 显示所有类别

    bool addCustomCategory(const string& category);// 添加自定义类别
    bool removeCustomCategory(const string& category);// 删除自定义类别

    // 检查是否为默认类别
    bool isDefaultCategory(const string& category) const;
    bool isCustomCategory(const string& category) const;// 检查是否为自定义类别

    int getCategoriesCount() const;// 获取类别总数

    bool saveCustomCategories() const;//*保存自定义类别到文件

private:
    string filename;//* 用于存储文件路径
    void loadCustomCategories();//* 从文件加载自定义类别
};

#endif // CATEGORYMANAGER_H