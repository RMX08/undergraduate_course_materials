#include<bits/stdc++.h>
using namespace std;

typedef long long LL;

// 扩展欧几里得算法： 求 ax + by = gcd(a, b) 的一组解 (x, y)
LL extended_gcd(LL a, LL b, LL &x, LL &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    //gcd(a,b) = gcd(b,a%b)
    LL d = extended_gcd(b, a % b, y, x);
    y -= a / b * x;

    /*
    exgcd(b, a % b, tx, ty); // tx, ty 是下一层的解 x', y'
    x = ty;                  // x = y'
    y = tx - a / b * ty;     // y = x' - (a/b)*y'
    */
   
    return d;// 返回值为 gcd(a, b)
}

vector<LL> linear_congruence(LL a, LL b, LL m)
{
    vector<LL> solutions;
    LL aa, bb, mm, x, y;

    //确保a，b，m为正数,否则C++标准中，负数模结果也为负数
    //利用同余的性质，将a，b映射到[0,m-1]的区间，结果不变
    a = (a % m + m) % m;
    b = (b % m + m) % m;

    //首先需要满足（a，m）| b
    LL d;
    d = extended_gcd(a,m,x,y);

    if (b % d != 0)//无解
        return solutions;

    //计算一个特解
    aa = a / d;
    bb = b / d;
    mm = m / d;
    LL x0;
    x0 = x * bb;//(a*x' + m*y') * (b/d) = d * (b/d)

    x0 = (x0 % mm + mm) % mm;

    //计算所有解
    for (int i = 0; i < d; i ++)
        solutions.push_back(x0 + i * mm);
    
    return solutions;

}

int main()
{
    //求ax + my = b 的解
    LL a, b, m;
    cout << "请输入ax ≡ b (mod m)： " << endl; //≡: alt + 41428
    cin >> a >> b >> m;

    vector<LL> solutions;
    solutions = linear_congruence(a,b,m);

    if (solutions.empty())
        cout << "该同余式无解！" << endl;
    else
    {
        cout << "该同余式的所有解为：" << endl;
        for (auto sol : solutions)
            cout << "x ≡ " << sol << "(mod " << m << ")" << endl;
    }

    return 0;

   

}
