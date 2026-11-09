#include<iostream>
#include<cmath>

using namespace std;

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp;
        temp = a % b;
        a = b;
        b = temp;
    }

    return a;
}

bool isPrime(int n)
{
    if (n == 1)
        return false;
    if (n == 2)
        return true;

    for (int i = 3; i <= sqrt(n); i += 2) 
    {
        if (n % i == 0) 
            return false;
    }    

    return true;
}

int qpow(int base, int exp, int mod) {
    int res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = res * base % mod;
        }
        base = base * base % mod;
        exp /= 2;
    }
    return res;
}

int Legendre(int a, int p) {
    a = a % p;
    if (a < 0) a += p;

    if (a == 0) return 0;

    // 3. 欧拉判别法: val = a^((p-1)/2) % p
    int val = qpow(a, (p - 1) / 2, p);

    if (val == p - 1)
        return -1; // 对应模 p 意义下的 -1
    else 
        return 1; 
    
}

int main()
{
    int a, p, answer;

    cout << "请输入勒让得符号（a/p）：" << endl;
    cout << "a = ";
    cin >> a;
    cout << endl << "p = ";
    cin >> p;

    if (gcd (p, a) != 1)
    {
        cout << "a与p不互素！" << endl;
        return 0;
    }
    else if (!isPrime(p) || p % 2 == 0)
    {
        cout << "p不是奇素数！" << endl;
    }

    answer = Legendre(a, p);
    cout << "( " << a << " / " << p << " ) = " << answer << endl;

    return 0;
}