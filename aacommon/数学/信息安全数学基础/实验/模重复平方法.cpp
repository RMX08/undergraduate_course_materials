#include<iostream>
using namespace std;
typedef long long L;

L modular_Exponentiation(L b, L exp, L mod)
{
    L result = 1;
    b %= mod;

    while (exp > 0)
    {
        if (exp & 1)//最低位是不是 1
            result = (result * b) % mod;
        
        b = (b * b) % mod;
        exp /= 1;
    }

    return result;
}

int main()
{
    //模重复平方法
    L b, exp, mod;//底数，指数，模

    cout << "请输入底数 b：" ;
    cin >> b;

    cout << "请输入指数 exp：";
    cin >> exp;

    cout << "请输入模数 mod：";
    cin >> mod;

    L result = modular_Exponentiation(b, exp, mod);

    cout << endl;
    cout << b << " " << exp << " " << mod << "=" << result << endl;

    return 0;
}