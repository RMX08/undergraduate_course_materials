#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;

// 欧拉函数
int get_euler(int m)
{
    int euler = 0;
    for (int i = 1; i < m; i++)
    {
        if (i % m == i)
            euler ++;
    }

    return euler;
}

// 所有因数
vector<int> get_all_factor(int euler)
{
    vector<int> factor;

    for (int i = 1; i <= sqrt(euler); i++)
    {
        if (euler % i == 0)
        {
            factor.push_back(i);
            factor.push_back(euler/i);
        }
    }

    sort(factor.begin(), factor.end());

    return factor;
}

int main()
{
    int a, m, euler, ord;
    vector<int> factor;

    cout << "请输入整数a: ";
    cin >> a;
    cout << "请输入模m: ";
    cin >> m;

    printf( "\n======== 计算整数%d模%d的指数ord%d(%d) ========\n\n", a,m,m,a);

    euler = get_euler(m);

    factor = get_all_factor(euler);

    for (int i = 0; i < factor.size(); i++)
    {
        if (int(pow(a,factor[i])) % m == 1)
        {
            ord = factor[i];
            printf("%d^%d ≡ 1 ( mod %d)\n", a,ord,m);
            printf("整数%d模%d的指数为: %d",a,m,ord);
            return 0;
        }
    }
}