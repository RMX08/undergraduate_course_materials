#include<iostream>

using namespace std;

int main()
{
    //判断同余：a ≡ b (mod m)
    int a,b,m;
    cin >> a >> b >> m;

    if ((a - b) % m == 0)
        cout << "a、b模m同余" << endl;
    else
        cout << "a、b模m不同余" << endl;

    return 0;
}