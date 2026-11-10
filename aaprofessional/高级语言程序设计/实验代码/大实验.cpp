#include<stdio.h>
int main()
{
	int y,m,flag=1;
	int maxday[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	printf("请输入1945年及以后的某一年及某一月（如：1949 10)\n");
	scanf("%d%d",&y,&m);
	//计算月份起始星期 
	for (int i=1945;i<=y;i++)
	{
		if ((i%4==0&&i%100!=0)||(i%400==0)) 
		    maxday[2]=29;
		else
		    maxday[2]=28;
		if (i!=y)//按年计算 
		{
			if (maxday[2]==28)
			    flag++;
			else
			    flag+=2;
			if (flag>7)
			    flag%=7;//下一年的第一天 
			//printf("%d %d\n",i,flag);
		}
		else//按月计算 
		{
			for (int j=1;j<m;j++)
			{
				flag+=maxday[j]%7;
				if (flag>7)
				    flag%=7; 
				//printf("%d %d %d\n",i,j,flag);
			}
		}
	}
	//输出
	//printf("%d\n",flag);
	printf("星期一\t星期二\t星期三\t星期四\t星期五\t星期六\t星期日\n");
	if (flag==2)
		printf("\t");
	else if (flag==3)
	    printf("\t\t");
	else if (flag==4)
	    printf("\t\t\t");
	else if (flag==5)
	    printf("\t\t\t\t");
	else if (flag==6)
	    printf("\t\t\t\t\t");
	else if (flag==7)
	    printf("\t\t\t\t\t\t");
	for (int i=1;i<=maxday[m];i++)
	{
		printf("  %d\t",i);
		flag++;
		if (flag>7)
		{
			printf("\n");
			flag%=7;
		}
	}
	return 0;
 } 
