#include<stdio.h>
int main()
{
	float f,c;
	printf("请输入一个华氏温度：");
	scanf("%f",&f);
	c=5.0*(f-32)/9;
	printf("该华氏温度对应的摄氏温度为：%f",c);
	return 0;
}
