#include<stdio.h>
#include<string.h>

char str[10];
int scoreh,scoreg;
int time1[5]={0},time2[5]={0};
int num[]={6,2,5,5,4,5,6,3,7,6};//每个分数的段数 

void scan()
{
	char people[10];
	scanf("%s",str);//score
	scanf("%d%d%d",&time2[1],&time2[2],&time2[3]);//score1 timee
    if (str[0]=='E') return; 
	scanf("%s",people);
    if (people[0]=='h')
        scanf("%d",&scoreh);
    else
        scanf("%d",&scoreg);
}

int main()
{
	while (~scanf("%s",str))
	{
		int ans=0,time;
		
		scanf("%d%d%d",&time1[1],&time1[2],&time1[3]);//start time
		scan();
		
        if (people)
		time=time_(time1,time2);
		ans+=6*time;
		
		int time3[5]={0};
		strcpy(str3,str1);
		strcpy(str1,str2);
		
		while (1)
		{
			scanf("%s",str);
			scanf("%d%d%d",&time2[1],&time2[2],&time2[3]);
			time=time_(time1,time2);
			sum+
			if (str[0]=='E')//flag
		}
		
	}
}
