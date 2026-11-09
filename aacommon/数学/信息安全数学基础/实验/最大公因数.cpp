#include<iostream>

using namespace std;
int main()
{
    //最大共因数:欧几里得算法
    int a,b;

    cin >> a >> b;

    while (b != 0)
    {
        int temp;
        temp = a % b;
        a = b;
        b = temp;
    }

    cout << "最大公因数为 " << a << endl;
    return 0;
}