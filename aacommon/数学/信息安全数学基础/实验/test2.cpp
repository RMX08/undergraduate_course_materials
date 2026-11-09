#include<bits/stdc++.h>
#include<vector>
using namespace std;
int main()
{
    long n,k,temp,num = 0, max = 0;
    vector<long> arr;

    cin >> n >> k;
    for (int i = 0; i < n; i++)
    {
        cin >> temp;
        if (temp >= 2000)
        {
            num = i;
            break;
        }
        arr.push_back(temp);
    }
    n = num;

    for (int i = 0; i < n-(k-1); i++)
    {
        long M;
        M = arr[n-k-i]-arr[0] + arr[n-1]-arr[n-i-1];
        cout << M <<endl;
        if (M > max)
            max = M;
        else
            break;
    }

    cout << max<<endl;
    return 0;


}