#include<iostream>
using namespace std;
int main()
{
    int m, euler = 0;
    cin >> m;

    for (int i = 1; i < m; i++)
    {
        if (i % m == i)
            euler ++;
    }

    cout << "euler(" << m << ") = " << euler << endl;

    return 0;
}