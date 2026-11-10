#include <iostream>

using namespace std;

int count(int n)
{
    if (n==1)
        return 1;
    else if (n==2)
        return 2;
    else
        return count(n-1)+count(n-2);
}

int main()
{
    int n;
    cin>>n;

    cout<<"There are "<<count(n)<<" ways to climb the staircase."<<endl;
    return 0;
}
/*
1  1
2  2
3  3
4  5
5  8
*/
