#include <iostream>

using namespace std;

const int MAX=100;
int arr[MAX],num[MAX][2]={0};
int n;

void _count()
{
    for (int i=0;i<n;i++)
    {
        if (!num[arr[i]][1])
        {
            cout<<arr[i]<<" 次数："<<num[arr[i]][0]<<" 下标：";
            num[arr[i]][1]=1;
            for (int j=i;j<n;j++)
            {
                if (arr[j]==arr[i])
                    cout<<j<<" ";
            }
            cout<<endl;
        }
    }
}

int main()
{
    cout<<"input n"<<endl;
    cin>>n;
    for (int i=0;i<n;i++)
    {
        cin>>arr[i];
        num[arr[i]][0]++;
    }

    _count();

    return 0;
}
