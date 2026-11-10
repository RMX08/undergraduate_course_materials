#include<stdio.h>
int main()
{
	int p1,p2,p3,p4,n,y,m,d,sum=0;
	int a[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	double ave;
	printf("TEST 05-1\n");
	printf("请输入四门课程的成绩：\n");
	scanf("%d%d%d%d",&p1,&p2,&p3,&p4);
	printf("获取的成绩信息为：\n");
	printf("%d %d %d %d\n",p1,p2,p3,p4);
	ave=(p1+p2+p3+p4)*1.0/4;
	printf("四门课程的平均分为：\n%.2lf ",ave);
	switch((int)ave/10)
	{
		case 10:
			printf("成绩优秀\n");
			break;
		case 9:
			if (ave>90)
			    printf("成绩优秀\n");
			else
			    printf("成绩良好\n");
			break;
		case 8:
		case 7:
			if (ave>70)
			    printf("成绩良好\n");
			else
			    printf("成绩及格\n");
			break;
		case 6:
		    printf("成绩及格\n");
		    break;
		default:
			printf("成绩不及格\n");
	}
	
	printf("\n\nTEST 05-2\n");
	printf("请输入一个整数：\n");
	scanf("%d",&n);
	if (n%2==0)
	    printf("该数为偶数\n");
	else
	    printf("该数为奇数\n");
	    
	printf("\n\nTEST 05-3\n");
	printf("请输入年月日：\n");
	scanf("%d%d%d",&y,&m,&d);
	//先判断是否为闰年 
	if (y%400==0 || y%100 && y%4==0)
	    a[2]++;
	for (int i=1;i<m;i++)//将该月之前的日子都加起来 
	    sum+=a[i];
	sum+=d;
	printf("这一天是这一年的第 %d 天\n",sum);
	
	printf("\n\nTEST 05-4\n");
	printf("请输入一个正整数：\n");
	scanf("%d",&n);
	if (n%5==0 && n%7==0)
	    printf("yes\n");
	else
	    printf("no\n");
	
	printf("\n\nTEST 05-5\n");
	printf("请输入一个整数：\n");
	scanf("%d",&n);
	if (n>=0)
	    printf("该数的绝对值为：%d\n",n);
	else
	    printf("该数的绝对值为：%d\n",-n);
	return 0;
 }
