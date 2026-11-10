#include<stdio.h>
#include<string.h>

void movement(char move[][15],int *p,int i)
{
	if (move[i][0]=='L')
		(*p)--;
	else if (move[i][0]=='R')
	    (*p)++;
	else
	{
		int len,poi=0;
		len=strlen(move[i]);
		if (len==9)
		    poi=move[i][8]-'0';
		else if (len==10)
		    poi=move[i][9]-'0'+(move[i][8]-'0')*10;
		movement(move,p,poi);
	}
}

int main()
{
	int T;
	scanf("%d",&T);
	
	while (T--)
	{
		int n,p=0;
		char move[101][15];
		scanf("%d",&n);
		getchar();
		
		for (int i=1;i<=n;i++)
		{
			gets(move[i]);
			if (move[i][0]=='L')
			    p--;
			else if (move[i][0]=='R')
			    p++;
			else
			    movement(move,&p,i);
		}
		
		printf("%d\n",p);
	}
	
	return 0;
 } 
