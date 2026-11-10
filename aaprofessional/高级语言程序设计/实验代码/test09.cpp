#include<cstdio>
int main()
{
	int flag,n,p,sit[21][7]={0};//0表示无人，1表示有人 sit[6]表示还剩几个座位 
	scanf("%d",&n);
	while (n--)
	{
		flag=0;
		scanf("%d",&p);
		for (int i=1;i<=20;i++)
		{
			if (5-sit[i][6]>=p)//表示该排剩余的位置可以容纳指令容量 
			{
				for (int j=1;j<=5;j++)
				{
					if (sit[i][j]==0)
					{
						sit[i][j]=1;
						p--;
						sit[i][6]++;//该排已有乘客数量加一 
						printf("%d ",5*(i-1)+j);
					}
					if (p==0)
					    break;
				 } 
				flag=1;
			}
			if (p==0)
			    break;
		}
		if (flag==0&&p!=0)//表示每一排都没法容纳完整的购票数量
		{
			for (int i=1;i<=20;i++)
			{
				if (sit[i][6]<5)
				{
					for (int j=1;j<=5;j++)
					{
						if (sit[i][j]==0)
						{
							sit[i][j]==1;
							p--;
							sit[i][6]++;
							printf("%d ",5*(i-1)+j);
						}
						if (p==0)
						    break;
					}
				}
				if (p==0)
				    break;
			}
		 } 
		printf("\n");
	}
	return 0;
 } 
