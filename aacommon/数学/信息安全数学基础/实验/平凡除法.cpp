#include<iostream>
#include<cmath>

#include<vector>
using namespace std;

//方法一：
bool isPrime(int n)
{
    if (n == 1)
        return false;
    if (n == 2)
        return true;

    for (int i = 3; i <= sqrt(n); i += 2) //只检查奇数
    {
        if (n % i == 0) //是合数
            return false;
    }    

    return true;
}

//方法二：
bool is_Prime(int n)
{
    if (n == 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    //先生成所有小于根号n的素数
    vector<int> prime;
    for (int i = 2; i <= sqrt(n); i ++)
    {
        if (isPrime(i))
            prime.push_back(i);
    }

    for (int p : prime)
    {
        if (n % p == 0)
            return false;
    }
    return true;
}

int main()
{
    //平凡除法：判断一个整数是不是素数
    int n;

    while (1)
    {
        cin >> n;
        if (n <= 0)
        {
            cout << "请输入一个大于0的整数" << endl;
            continue;
        }
        else 
            break;            
    }

    //方法一：if (isPrime(n))
    if (is_Prime(n))//方法二
        cout << n << "是素数" << endl;
    else
        cout << n << "不是素数" << endl;
    
    return 0;
    
}