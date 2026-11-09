#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

typedef long long ll;
typedef vector<ll> Poly;

ll mod(ll a, ll m)
{
    return (a%m + m) % m;
}

Poly readPoly(string name, ll m)
{
    ll d, coe, exp, len;
    cout << "请输入 " << name << "最高次数 ：";
    cin >> d;

    if (d < 0) return {};

    Poly P(d+1);
    cout << "请输入多项式的长度：";
    cin >> len;
    cout << "请依次输入多项式的系数和指数 (从高到低）： "  << endl;

    for(ll i = 0; i < len ; i++)
    {
        cin  >> coe >> exp;
        P[exp] = coe;
        // P[exp] = mod(coe, m);
    }

    return P;
}

void printPoly(Poly P)
{
    if (P.empty())
    {
        cout << "0" << endl;
        return ;
    }

    int flag = 1;
    for (ll i = P.size() - 1; i >= 0; i--)
    {
        
        if (P[i] == 0)
            continue;
        if (flag)
        {
            cout << P[i] << "x^" << i ;
            flag = 0;
            continue;
        }

        if (!flag && P[i] > 0)
        {
            cout << " + " << P[i];
            if (i > 0)
                cout << "x^" << i ;
        }
        else if (!flag && P[i] < 0)
        {
            cout << " - " << -P[i] ;
            if (i > 0)
                cout << "x^" << i;
        }
    }

    if (P.size() == 1 && P[0] == 0)
        cout << "0";

    cout << endl;
    return ;
}

//拓展欧几里得算法
ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll d = exgcd(b, a%b, y, x);
    y -= a/b * x;
    return d;
}

//求逆元
ll modInverse(ll a, ll m)
{
    ll x, y;
    ll d = exgcd(a, m, x, y);
    if (d != 1) return -1;
    return mod(x, m);
}

//求最高次数
int deg(const Poly &P)
{
    if (P.empty()) return -1;

    return P.size() - 1;
}

// 去除多项式高位的0
void trim(Poly &P)
{
    while (P.size() > 0 && P.back() == 0)
        P.pop_back();
}

pair<Poly, Poly> polyDivMod(Poly A, Poly B, ll m)
{
    ll leadB = B.back();
    ll invleadB = modInverse(leadB, m);// 求逆元
    if(invleadB == -1)
    {
        cout << "错误：除式的最高项系数 (" << leadB << ") 在模 " << m << " 下不可逆" << endl;
        cout << "无法进行欧几里得除法!!" << endl;
        return {{},{}};
    }

    int MAX = max((int)A.size(), 1);
    Poly Q(MAX, 0);
    Poly R = A;

    while (deg(R) >= deg(B))
    {
        int diff = deg(R) - deg(B);//最高次数相差
        ll leadR = R.back();//最高项系数
        ll factor = mod(leadR * invleadB, m);//计算当前商的系数 factor = leadR / leadB

        if (diff >= Q.size()) 
            Q.resize(diff + 1, 0);
        Q[diff] = mod(Q[diff] + factor, m);

        for (ll i = 0; i < B.size(); i++) 
        {
            ll sub = mod(factor * B[i], m);
            R[i + diff] = mod(R[i + diff] - sub, m);
        }
        //此时 R 的最高位一定是 0，需要去掉它以更新 deg(R)
        trim(R);
    }

    trim(Q);
    return {Q, R};
    
}

int main()
{
    Poly A, B; //A 被除式 B 除式，下标 i 对应 xi 的系数
    ll lena, lenb, m;

    cout << "请输入模数 m ：" << endl;
    cin >> m;
    
    cout << "---请输入多项式 A(x)--- " << endl;
    A = readPoly("A(x)", m);
    cout << "---请输入除式 B(x)--- " << endl;
    B = readPoly("B(x)", m);
    
    cout << "输入的多项式A(x) : ";
    printPoly(A);
    cout << "输入的多项式B(x) : ";
    printPoly(B);

    pair<Poly, Poly> result;
    result = polyDivMod(A, B, m);

    if (result.first.empty() && result.second.empty() && !B.empty()) 
        return 1;
    
    cout << endl << endl;
    cout << "商 Q(x) = " ;
    printPoly(result.first);
    cout << "余数 R(x) = ";
    printPoly(result.second);

    return 0;
}