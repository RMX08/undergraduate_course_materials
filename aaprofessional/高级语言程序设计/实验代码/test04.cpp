#include<stdio.h>
#define pi 3.1415926
int main()
{
	double r,up,ave;
	int s1,s2,s3,s4;
	//TEST 1
	printf("TEST 04-1\n");
	scanf("%lf",&r);
	printf("该圆的周长为：%.3lf\n",2*pi*r);
	printf("该圆的面积为：%.3lf\n",pi*r*r);
	printf("该圆组成球的体积为：%.3lf\n",4*1.0/3*pi*r*r*r);
	//TEST 2
	printf("\n\nTEST 04-2\n");
	scanf("%d%d%d%d",&s1,&s2,&s3,&s4);
	printf("各季度自年初起的累计销售额：\n");
	printf("第一季度：%d\n",s1);
	printf("第二季度：%d\n",s1+s2);
	printf("第三季度：%d\n",s1+s2+s3);
	printf("第四季度：%d\n",s1+s2+s3+s4);
	printf("后三个季度的环比增长率：\n");
	up=(s2-s1)*1.0/s1*100;
	printf("第二季度：%.2lf%%\n",up);
	up=(s3-s2)*1.0/s2*100;
	printf("第三季度：%.2lf%%\n",up);
	up=(s4-s3)*1.0/s3*100;
	printf("第四季度：%.2lf%%\n",up);
	//TEST 3
	printf("\n\nTEST 04-3\n");
	printf("请输入四门课程的分数：\n");
	scanf("%d%d%d%d",&s1,&s2,&s3,&s4);
	printf("四门分数为：%d %d %d %d\n",s1,s2,s3,s4);
	ave=(s1+s2+s3+s4)*1.0/4;
	printf("四门平均分为：%.2lf\n",ave);
	return 0;
 } 
