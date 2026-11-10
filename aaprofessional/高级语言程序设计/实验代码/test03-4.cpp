#include<stdio.h>
int main()
{
	int a,A;//a为小写字母的ASCll码值 A为大写字母的ASCll值 
	char x,X;
	scanf("%c",&x);
	a=x;
	X=x-'a'+'A';
	A=X;
	printf("%c\t%c\n%d\t%d",x,X,a,A);
	return 0; 
}
