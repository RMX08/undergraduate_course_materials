#include<stdio.h>
#include<string.h>
int main()
{
	char *str1,string1[100],string2[100];
	char *str;
	scanf("%s%s",string1,string2);
	str1=string1;
	str=strstr(string1,string2);
	if (*str==NULL)
	    printf("-1\n");
	else
	{
		for (int i=0;i<strlen(string1);i++)
		{
			if (str1+i==str)
			{
			    printf("%d\n",i);
			    break;
			}
		}
	}
	return 0;
 } 
