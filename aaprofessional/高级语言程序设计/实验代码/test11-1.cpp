#include<stdio.h>
int _gcd(int a,int b)
{
	int c,gcd;
	if (b==0)
	    return a;
	else
	{
		c=a%b;
		//printf("%d %d %d\n",a,b,c);
		a=b;
		b=c;
		gcd=_gcd(a,b);
	}
}
int main() 
{
	int a,b,x,y,gcd;
	scanf("%d%d",&a,&b);
	printf("两数的最大公约数为： ");
	x=a>b?a:b;
	y=(a+b)-x;
	gcd=_gcd(x,y); 
	printf("%d",gcd);
	return 0;
}
