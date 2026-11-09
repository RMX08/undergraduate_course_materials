#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 最大公约数
int gcd(int a, int b)
{
    while (b) 
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 欧拉函数
int euler(int n) 
{
    int euler = 0;
    for (int i = 1; i < n; i++)
    {
        if (i % n == i)
            euler ++;
    }

    return euler;
}

// 质因数分解（返回所有质因数，含重复）
vector<int> get_factor(int n) 
{
    vector<int> factor;

    for (int i = 2; i * i <= n; ++i) 
    {
        while (n % i == 0) 
        {
            factor.push_back(i);
            n /= i;
        }
    }
    if (n > 1)
        factor.push_back(n);

    return factor;
}

// 生成所有正约数（去重、排序）
vector<int> get_divisor(int n) 
{
    vector<int> factor = get_factor(n);
    vector<int> divisor = {1};

    for (int p : factor) 
    {
        int sz = divisor.size();
        for (int i = 0; i < sz; ++i) 
            divisor.push_back(divisor[i] * p);
    }

    sort(divisor.begin(), divisor.end());
    divisor.erase(unique(divisor.begin(), divisor.end()), divisor.end());

    return divisor;
}

// 快速幂取模
long long quick_pow_mod(long long base, long long exp, long long mod) 
{
    long long res = 1;
    base %= mod;
    while (exp > 0) 
    {
        if (exp % 2 == 1) 
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

// 计算a模m的乘法阶（要求gcd(a,m)=1）
int order(int a, int m) 
{
    if (gcd(a, m) != 1)
        return -1; // 不互质则无乘法阶

    int phi = euler(m);
    vector<int> divisor = get_divisor(phi);
    for (int d : divisor) 
    {
        if (quick_pow_mod(a, d, m) == 1) 
            return d;
    }
    return phi; 
}

// 计算序列的最小周期
int min_period(int a, int m) {
    int current_a = a;
    int current_m = m;
    int t = 0;

    // 找到最小的t，使得gcd(current_a, current_m) = 1
    while (gcd(current_a, current_m) != 1) 
    {
        int g = gcd(current_a, current_m);
        current_m /= g;
        current_a = (long long)current_a * a % m; // 计算a^(t+1)
        t++;
        if (current_m == 1) 
            return 1; // 模1结果恒为0，周期为1
    }

    // 计算current_a模current_m的乘法阶
    return order(current_a, current_m);
}

int main() {
    int a, m;
    cout << "请输入a和m：";
    cin >> a >> m;
    int p = min_period(a, m);
    cout << "序列的最小周期p(u)为：" << p << endl;
    return 0;
}