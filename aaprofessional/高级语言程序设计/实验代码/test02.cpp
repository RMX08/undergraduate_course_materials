#include<stdio.h>
#include<cmath>
int main()
{
	int a;
	scanf("%d",&a);
	for (int i=2;i<=sqrt(a);i++)
	{
		if (a%i==0)
		{
			printf("不是素数");
			return 0;
		}
	}
    printf("是素数");
	return 0; 
 } 
