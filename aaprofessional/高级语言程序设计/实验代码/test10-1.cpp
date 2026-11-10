#include<stdio.h>
#include<string.h>
void compare1(char s1[],int n)//敏感 
{
	int m=0;//flag=1表示在查找中 
	char s3[101][100];
	while (n--)
	{
		int a=0,flag=0;
		char s2[1000];
		scanf("%s",s2);
		//printf("%s %d\n",s1,strlen(s1));
		for (int i=0;i<=strlen(s2);i++)
		{
			if (a==strlen(s1))
			{
				//printf("YES~\n");
				for (int j=0;j<strlen(s2);j++)
				    s3[m][j]=s2[j];
				m++;
				break;
			}
			if (a!=0&&a<strlen(s1))
			{
				if (s2[i]==s1[a])
				    a++;
				else
				{
					a=0;
					flag=0; 
				} 
			}
			if (s2[i]==s1[0]&&flag==0)
			{
				a=1;
				flag=1;
			} 
			//printf("a=%d flag=%d\n",a,flag);
		}
		//printf("%d\n",m);
	}
	for (int i=0;i<m;i++)
	    printf("%s\n",s3[i]);
}
void compare0(char s1[],int n)
{
	int m=0;
	char s3[102][100];
	while (n--)
	{
		int a=0,flag=0;
		char s2[1000];
		scanf("%s",s2);
		for (int i=0;i<=strlen(s2);i++)
		{
			if (a==strlen(s1))
			{
				for (int j=0;j<strlen(s2);j++)
				    s3[m][j]=s2[j];
				break;
			}
			if (a!=0&&a<strlen(s1))
			{
				if (s2[i]==s1[a]||s2[i]==(s1[a]-32)||s2[i]==(s1[a]+32))
				    a++;
				else
				{
					a=0;
					flag=0; 
				} 
			}
			if ((s2[i]==s1[0]||s2[i]==(s1[0]+32)||s2[i]==(s1[0]-32))&&flag==0)
			{
				a=1;
				flag=1;
			} 
			//printf("a=%d flag=%d\n",a,flag);
		}
		//printf("%d\n",m);
	}
	for (int i=0;i<m;i++)
	    printf("%s\n",s3[i]);
}
int main()
{
	char s1[1000];
	int sel,n;
	scanf("%s",s1);
	scanf("%d%d",&sel,&n);
	if (sel==0)
	    compare0(s1,n);
	else
	    compare1(s1,n);
	return 0;
 } 
