#include<stdio.h>
int fb(int n)
{
    if (n==1)
        return 1;
    else if (n==2)
        return 2;
    else
        return fb(n-1)+fb(n-2);
}
int main()
{
    int T,n,sum=0;
    scanf("%d",&T);
    while (T--)
    {
        scanf("%d",&n);
        sum=fb(n);
        printf("%d\n",sum);
    }
    return 0;
}
