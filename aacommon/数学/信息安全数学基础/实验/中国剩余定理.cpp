#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

// 扩展欧几里得算法： 求 ax + by = gcd(a, b) 的一组解 (x, y)
ll exgcd(ll a, ll b, ll &x, ll &y) 
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    //gcd(a,b) = gcd(b,a%b)
    ll d = exgcd(b, a % b, y, x);
    y -= a / b * x;

    /*
    exgcd(b, a % b, tx, ty); // tx, ty 是下一层的解 x', y'
    x = ty;                  // x = y'
    y = tx - a / b * ty;     // y = x' - (a/b)*y'
    */

    return d;// 返回值为 gcd(a, b)
}


// 求 a逆元,即求解 ax ≡ 1 (mod m)
ll modInverse(ll a, ll m) 
{
    ll x, y;
    ll d = exgcd(a, m, x, y);
    if (d != 1) {
        return -1; //不存在逆元
    }
    return (x % m + m) % m; // 保证结果为正数
}


// 中国剩余定理 CRT
ll chinese_remainder_theorem(const vector<ll>& b, const vector<ll>& m, ll n, ll M) 
{
    ll result = 0;
    
    // 公式 x = Σ (ai * Mi * ti) (mod M)
    for (int i = 0; i < n; i++) 
    {
        ll Mi = M / m[i]; 
        ll ti = modInverse(Mi, m[i]); // ti 是 Mi 的逆元
        
        // 防止中间结果溢出，每一步都取模，但公式是在求和后取模。        
        ll term = b[i] % M;
        term = (term * (Mi % M)) % M;
        term = (term * (ti % M)) % M;
        
        result = (result + term) % M;
    }

    // 保证结果是正最小余数
    return (result % M + M) % M;
}

int main() 
{
    //232 357
    vector<ll> b; // 余数数组
    vector<ll> m; // 模数数组

    ll n, M = 1;
    cout << "请输入方程组个数：" << endl;
    cin >> n;
    cout << "请输入方程组对应的所有余数：" << endl;
    for (int i = 0 ;i < n; i++)
    {
        ll bb;
        cin >> bb;
        b.push_back(bb);
    }
    cout << "请输入所有的模数：" << endl;
    for (int i = 0 ;i < n; i++)
    {
        ll mm;
        cin >> mm;
        m.push_back(mm);
    }

    for (auto i : m)
        M *= i;

    ll x = chinese_remainder_theorem(b, m, n, M);

    cout << "该同余式方程组为:" << endl;
    for(size_t i = 0; i < n; ++i) {
        cout << "x = " << b[i] << " (mod " << m[i] << ")" << endl;
    }
    
    cout << "同余式组的解为： x ≡ " << x << "( mod " << M << " )" << endl;

    return 0;
}