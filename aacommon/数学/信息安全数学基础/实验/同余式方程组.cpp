#include <bits/stdc++.h>

using namespace std;

typedef long long LL;


LL extended_gcd(LL a, LL b, LL &x, LL &y)
{
    if (a == 0)
    {
        x = 0;
        y = 1;
        return b;
    }

    LL x1, y1;
    LL d = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b/a) * x1;
    y = x1;

    return d;
}


void congruence_equation(LL b1, LL b2, LL m1, LL m2)
{
    LL m, M1, M2, M11, M22, x, d;

    if (m1 > m2) swap(m1,m2);

    m = m1 * m2;
    M1 = m2;
    M2 = m1;

    //判断是否有解，m1和m2是否互素
    LL a = m1, b = m2;
    while (b != 0)
    {
        int temp;
        temp = a % b;
        a = b;
        b = temp;
    }
    if (a != 1)
    {
        cout << "该同余式方程组无解！" << endl;
        return ;
    }

    //计算M1，M2逆元
    d = extended_gcd(M2, m2, M22, M11);

    //求解
    x = b1*M1*M11%m + b2*M2*M22%m ; 
    if (x < 0)
        x = m + x;

    cout << "该同余式方程组的解为 x ≡ " << x << " (mod " << m << ")";
    return;
}

int main()
{
    LL b1, b2, m1, m2;

    cout << "请输入同余式方程组：" << endl;
    cout << "x ≡ b1 (mod m1)" << endl;
    cout << "x ≡ b2 (mod m2)" << endl;

    cout << "b1 = ";
    cin >> b1;
    cout << "b2 = ";
    cin >> b2;
    cout << "m1 = ";
    cin >> m1;
    cout << "m2 = ";
    cin >> m2;

    congruence_equation(b1, b2, m1, m2);

    return 0;
}