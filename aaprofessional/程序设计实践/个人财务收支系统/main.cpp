#include <iostream>
#include <limits>
#include <cstdlib>  // 用于跨平台清屏
#include "FinanceManager.h"
#include "UserInterface.h"
#include "FileHandler.h"

using namespace std;

int main() {

    try {
        FileHandler fileHandler("transactions.txt");
        int financialTarget = 0;
        int loadedTarget = fileHandler.readFinancialTarget();

        if (loadedTarget == -1) {
            cout << "欢迎使用个人财务收支管理系统！\n";
            cout << "请输入您的财务自由目标金额（正数）：";

            // 输入验证
            while (!(cin >> financialTarget) || financialTarget <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效！请重新输入正数金额：";
            }
            fileHandler.saveFinancialTarget(financialTarget);
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲
        }
        else {
            cout << "当前已设置的财务目标：" << loadedTarget << " 元\n";
            cout << "是否需要修改财务目标？(y/n)：";

            char modifyChoice;
            cin >> modifyChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 关键修复：清除残留换行符

            if (modifyChoice == 'y' || modifyChoice == 'Y') {
                cout << "请输入新的财务目标金额：";
                while (!(cin >> financialTarget) || financialTarget <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "输入无效！请重新输入正数金额：";
                }
                fileHandler.saveFinancialTarget(financialTarget);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                financialTarget = loadedTarget;
            }
        }

        system("cls");
        FinanceManager financeManager(financialTarget, fileHandler);
        UserInterface ui(&financeManager);

        // 带退出机制的主循环
        bool isRunning = true;
        while (isRunning) {
            ui.showMainMenu();
            int menuOption = ui.getValidMenuOption();
            isRunning = ui.handleMenuOption(menuOption); // 现在 handleMenuOption 返回 bool
        }

        cout << "感谢使用，再见！\n";

    }
    catch (const exception& e) {
        cerr << "程序运行出错：" << e.what() << endl;
        cerr << "请检查文件权限或联系技术支持\n";
        system("pause");
        return 1;
    }

    return 0;
}
