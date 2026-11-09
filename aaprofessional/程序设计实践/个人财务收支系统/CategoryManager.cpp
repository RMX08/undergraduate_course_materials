#include "CategoryManager.h"
#include <iostream>
#include<fstream>
CategoryManager::CategoryManager(const std::vector<std::string>& default_cats, const std::string& filepath) : filename(filepath)
{
    for (const auto& cat : default_cats) {
        default_categories.insert(cat);
    }
    //构造时立即加载已保存的自定义类别
    loadCustomCategories();
}
//加载
void CategoryManager::loadCustomCategories()
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string category;
    while (std::getline(file, category)) {
        if (!category.empty()) {
            custom_categories.insert(category);
        }
    }
    file.close();
}
// 新增：实现保存方法
bool CategoryManager::saveCustomCategories() const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开类别文件进行写入: " << filename << std::endl;
        return false;
    }

    for (const auto& category : custom_categories) {
        file << category << std::endl;
    }

    file.close();
    return true;
}

std::vector<std::string> CategoryManager::getAllCategories() const {
    std::vector<std::string> all_categories;

    for (const auto& category : default_categories)
        all_categories.push_back(category);

    for (const auto& category : custom_categories)
        all_categories.push_back(category);

    return all_categories;
}

bool CategoryManager::isDefaultCategory(const std::string& category) const {
    return default_categories.find(category) != default_categories.end();
}

bool CategoryManager::isCustomCategory(const std::string& category) const {
    return custom_categories.find(category) != custom_categories.end();
}

bool CategoryManager::addCustomCategory(const std::string& category) {
    if (isDefaultCategory(category) || isCustomCategory(category))
        return false;
    
    custom_categories.insert(category);
    return true;
}

bool CategoryManager::removeCustomCategory(const std::string& category) {
    if (isDefaultCategory(category)) // 不删除默认类别
        return false;
    
    if (isCustomCategory(category)) {
        custom_categories.erase(category);
        return true;
    }
    else
        return false;
}

void CategoryManager::displayAllCategories(const std::string& type) const {
    std::cout << "当前默认" << type << "类别：" << std::endl;
    for (const auto& ctg : default_categories)
        std::cout << " - " << ctg << std::endl;

    std::cout << "当前自定义" << type << "类别：" << std::endl;
    if (custom_categories.empty())
        std::cout << " - 无 " << std::endl;
    else {
        for (const auto& ctg : custom_categories)
            std::cout << " - " << ctg << std::endl;
    }
}

int CategoryManager::getCategoriesCount() const {
    return default_categories.size() + custom_categories.size();
}