#include<iostream>
#include<cmath>
#include<algorithm>

using namespace std;

bool prime(int p)
{
    for (int i = 2; i <= sqrt(p); i ++)
    {
        if (p % i == 0)
            return false;
    }
    return true;
}

int gcd(int a, int b)
{
    if (a < b) swap(a,b);

    while (b != 0)
    {
        int temp;
        temp = a%b;
        a = b;
        b = temp;
    }

    return a;
}

int main()
{
    int a, p, d;
    bool pp;

    cin >> a >> p;

    if (p % 2 == 0)
    {
        printf("p不为奇数！！不满足欧拉判别条件");
        return 0;
    }
    
    pp = prime(p);
    if (!pp)
    {
        printf("p不为素数！不满足欧拉判别条件");
        return 0;
    }

    d = gcd(a,p);
    if (d != 1)
    {
        printf("a和p不互素！不满足欧拉判别条件！");
        return 0;
    }

    if (int(pow(double(a),double((p-1)/2))) % p == 1)
        printf("%d是模%d的平方剩余",a,p);
    else
        printf("%d是模%d的平方非剩余",a,p);

    return 0;

}