#include<cstdio>
#include<iostream>
using namespace std;
void TEST1()
{
	int n,a[100000]={0},b[100000]={0};
	printf("TEST 07-1\n");
	printf("请输入商店个数：\n");
	scanf("%d",&n);
	printf("请输入各个商店第一天的菜价：\n");
	for (int i=1;i<=n;i++)
	    scanf("%d",&a[i]);
	b[1]=(a[1]+a[2])/2;
	b[n]=(a[n]+a[n-1])/2;
	for (int i=2;i<=n-1;i++)
		b[i]=(a[i-1]+a[i]+a[i+1])/3;
	printf("第二天各个商店的菜价：\n");
	for (int i=1;i<=n;i++)
	    printf("%d ",b[i]);
	return;
}
int MAX(int x,int y)
{
	return x>y?x:y;
}
int MIN(int x,int y)
{
	return x<y?x:y;
}
void TEST2()
{
	int n,max=0,min=99999,a;
	printf("\n\nTEST 07-2\n");
	printf("请输入序列长度： ");
	scanf("%d",&n);
	printf("\n请输入序列\n");
	for (int i=0;i<n;i++) 
    {
        scanf("%d",&a);
	    max=MAX(a,max);
	    min=MIN(a,min);
	}
	printf("最大值为 %d\n",max);
	printf("最小值为 %d\n",min);
	return;
}
void TEST3()
{
	int n,a[100000];
	printf("\n\nTEST 07-3\n");
	printf("请输入序列个数：");
	scanf("%d",&n);
	printf("\n请输入序列\n");
	for (int i=0;i<n;i++)
	    scanf("%d",&a[i]);
	for (int i=0;i<n-1;i++)
	  for (int j=0;j<n-i-1;j++)
	  {
	  	if (a[j]>a[j+1])
	  	{
	  		int tmp;
	  		tmp=a[j];
	  		a[j]=a[j+1];
			a[j+1]=tmp; 
		}
	  }
	for (int i=0;i<n;i++)
	    printf("%d ",a[i]);
	return;
}
void TEST4()
{
	int a[3][4],b[4][3],c[3][3]={0};
	printf("\n\nTEST 07-4\n");
	for (int i=0;i<2;i++)
	  for (int j=0;j<3;j++)
	    scanf("%d",&a[i][j]);
	for (int i=0;i<3;i++)
	  for (int j=0;j<2;j++)
	    scanf("%d",&b[i][j]);
	for (int i=0;i<2;i++)
	  for (int j=0;j<2;j++)
	  {
	  	for (int p=0;p<3;p++)
	  		c[i][j]+=a[i][p]*b[p][j];
	  }
	for (int i=0;i<2;i++)
	{
	    for (int j=0;j<2;j++)
	      printf("%d ",c[i][j]);
	    printf("\n");
	}
	return;
}
int main()
{
	TEST1();
	TEST2();
	TEST3();
	TEST4();
	return 0;
 } 
