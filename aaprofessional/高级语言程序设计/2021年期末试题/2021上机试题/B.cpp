#include<stdio.h>
#include<math.h>

int ans(double ob[],int n,int len)
{
    double m;
    int sum=0;
    m=ob[0];
    for (int i=1;i<len;i++)
    {
        int flag=0;
        if (ob[i]==m)
        {
            sum++;
            m=ob[i];
        }
        else if (ob[i]>m)//向下
        {
            int a;
            a=ceil(ob[i]-m);
            n-=a;
            m=ob[i];
            if (n>0)
                sum++;
            else 
                flag=1;
        }
        else//向上
        {
            int a;
            a=ceil(ob[i]-m);
            n-=a*2;
            m=ob[i];
            if (n>0)
                sum++;
            else 
                flag=1;
        }
        if (flag)
            return sum;
    }
}

int main()
{
    int n;
    double ob[100000]={0};
    scanf("%d",&n);

    int i=0;
    char ch;
    ch=getchar();
    while(ch!=']')
    {
        scanf("%lf",&ob[i++]); 
        ch=getchar();
        printf("%d",i);
    }

    printf("%d",ans(ob,n,i));

    return 0;
}
