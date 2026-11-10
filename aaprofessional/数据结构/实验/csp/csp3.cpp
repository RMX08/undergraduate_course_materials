#include<iostream>
using namespace std;
int main()
{
    int n;
    int a[1001], b[1001];
    cin >> n;

    for (int i = 0; i <= n; i++)
        cin >> a[i];
    
    b[0] = 0;
    for (int i = 1; i <= n; i++)
        cin >> b[i];

    for (int j = 1; j <= n; j++)
    {
        int w = 0,temp = 0,cur = 0;
        temp = b[j];
        b[j] = 0;
        for (int i = 0 ;i <= n; i++)
        {
            int w0 = 0;
            w0 = a[i] - b[i];
            if (cur < w0)
            {
                w = w + w0;
                cur = 0;
                // cout << w << " "<< cur<<endl;
            }
            else
            {
                cur = cur - w0;
                // cout<<cur<<endl;
            }
        }
        cout << w << " ";
        b[j] = temp;
    }

    return 0;
}