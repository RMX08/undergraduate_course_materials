// 请从此处开始编码 
#include<cstdio>

int r=0,c=0;

void wrong(int arr[][501],int n,int m)
{
    int max=0;
    for (int i=1;i<=n;i++)
    {
        int sum=0;
        for (int j=1;j<=m;j++)
            sum+=arr[i][j];
        printf("i:  %d  sum: %d\n",i,sum);
        if (sum>max)
        {
            max=sum;
            r=i;
        }
    }
    max=0;
    for (int j=1;j<=m;j++)
    {
        long long sum=0;
        for (int i=1;i<=n;i++)
            sum+=arr[i][j];
        printf("j:  %d  sum: %d\n",j,sum);
        if (sum>max)
        {
            max=sum;
            c=j;
        }
    }
}

void acc(int arr[][501],int n,int m)
{
    int max=0;
    int sumr[501]={0},sumc[501]={0};
    for (int i=1;i<=n;i++)
        for (int j=1;j<=m;j++)
            sumr[i]+=arr[i][j];
    for (int j=1;j<=m;j++)
        for (int i=1;i<=n;i++)
            sumc[j]+=arr[i][j];
    for (int i=1;i<=n;i++)
        for (int j=1;j<=m;j++)
        {
        	printf("i: %d  j: %d  sum: %d\n",i,j,sumr[i]+sumc[j]-arr[i][j]);
            if ((sumr[i]+sumc[j]-arr[i][j])>max)
            {
                r=i;
                c=j;
                max=sumr[i]+sumc[j]-arr[i][j];
            }
        }
}

int main()
{
    int n,m;
    int sum=0;
    while (~scanf("%d%d",&n,&m))
    {
        sum++;
        int arr[501][501]={0};
        for (int i=1;i<=n;i++)
            for (int j=1;j<=m;j++)
                scanf("%d",&arr[i][j]);
        int r1,r2,c1,c2;
        printf("wrong:\n");
        wrong(arr,n,m);
        r1=r;
        c1=c;
        printf("\nr1 %d  c1 %d\n\n",r,c);
        printf("acc:\n");
        acc(arr,n,m);
        r2=r;
        c2=c;
        printf("\nr2 %d  c2 %d\n\n",r,c);
        if (r1==r2 && c1==c2)
            printf("Case %d: Weak\n",sum);
        else
            printf("Case %d: Strong\n",sum);
    }
    
    return 0;
}
