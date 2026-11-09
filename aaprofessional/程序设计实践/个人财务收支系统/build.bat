@echo off
echo 正在编译个人财务收支系统...

REM 检查源文件是否存在
if not exist "main.cpp" (
    echo 错误：找不到 main.cpp
    pause
    exit /b 1
)

if not exist "UserInterface.cpp" (
    echo 错误：找不到 UserInterface.cpp
    pause
    exit /b 1
)

REM 编译所有源文件
echo 编译 main.cpp...
g++ -c main.cpp -o main.o
if %errorlevel% neq 0 (
    echo main.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 UserInterface.cpp...
g++ -c UserInterface.cpp -o UserInterface.o
if %errorlevel% neq 0 (
    echo UserInterface.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 FinanceManager.cpp...
g++ -c FinanceManager.cpp -o FinanceManager.o
if %errorlevel% neq 0 (
    echo FinanceManager.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 Transaction.cpp...
g++ -c Transaction.cpp -o Transaction.o
if %errorlevel% neq 0 (
    echo Transaction.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 Income_Transaction.cpp...
g++ -c Income_Transaction.cpp -o Income_Transaction.o
if %errorlevel% neq 0 (
    echo Income_Transaction.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 Expense_Transaction.cpp...
g++ -c Expense_Transaction.cpp -o Expense_Transaction.o
if %errorlevel% neq 0 (
    echo Expense_Transaction.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 Filehandler.cpp...
g++ -c Filehandler.cpp -o Filehandler.o
if %errorlevel% neq 0 (
    echo Filehandler.cpp 编译失败！
    pause
    exit /b 1
)

echo 编译 CategoryManager.cpp...
g++ -c CategoryManager.cpp -o CategoryManager.o
if %errorlevel% neq 0 (
    echo CategoryManager.cpp 编译失败！
    pause
    exit /b 1
)

REM 链接生成可执行文件
echo 正在链接生成可执行文件...
g++ -std=c++17 -o finance_system.exe main.o UserInterface.o FinanceManager.o Transaction.o Income_Transaction.o Expense_Transaction.o Filehandler.o CategoryManager.o
if %errorlevel% neq 0 (
    echo 链接失败！
    pause
    exit /b 1
)

echo.
echo 编译成功！生成了 finance_system.exe
echo.
echo 文件时间戳：
dir finance_system.exe | findstr "finance_system.exe"
pause
