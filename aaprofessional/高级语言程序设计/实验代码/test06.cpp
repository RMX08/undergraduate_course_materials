#include<cstdio>
#define pi 3.1415926
int main()
{
	double S,ave=0;
	int ch,n=0;
	long long ans=1; 
	//TEST 06-1
	printf("TEST 06-1\n");
	for (int i=1;i<=15;i++)
	{
		S=pi*i*i;
		if (30<=S && S<=100)
			printf("半径为%d的圆面积为：%.3lf\n",i,S);
	}
	//TEST 06-2
	printf("\nTEST 06-2\n");
	printf("九九乘法表\n");
	for (int i=1;i<=9;i++)
	{
	    for (int j=1;j<=i;j++)
	    {
	  	    printf("%d*%d=%d\t",i,j,i*j);
	    }
	    printf("\n");
	}
	//TEST 06-3
	printf("\nTEST 06-3\n");
	printf("200以内能被7整除的数字：\n");
	for (int i=1;i<=200;i++)
		if (i%7==0)
		    printf("%d ",i);
	//TEST 06-4
	printf("\n\nTEST 06-4\n");
	while (1)
	{
		scanf("%d",&ch);
		if (ch==0)
		    break;
		else
		{
		    if (ch%2==0)
		        ans*=ch;
		    else
		    {
		        ave+=ch;
		        n++;
		    }
		}
	}
	printf("奇数的平均数为 %.3lf\n",ave*1.0/n);
	printf("偶数的乘积为 %d\n",ans);
	
	return 0;
 } 
