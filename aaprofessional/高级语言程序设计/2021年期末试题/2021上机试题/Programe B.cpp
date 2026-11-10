#include<stdio.h>
#include<string.h>

bool cool(char str[])
{
	int num[30]={0};
	int sum[31]={0};
	for (int i=0;i<strlen(str);i++)//计算每个字母出现个数 
		num[str[i]-'a']++;
	for (int i=0;i<26;i++)//计算数量是否重复 
	{ 
	    if (num[i])
	    	sum[num[i]]++;
		if (sum[num[i]]>1)
		    return false; 
    } 
	return true;
}

int main()
{
	int n;
	int sum=0;
	
	while (~scanf("%d",&n))
	{
		int ans;
		char str[31];
		bool judge;
		ans=n;
		sum++;
		for (int i=0;i<n;i++)
		{
			scanf("%s",str);
			if (strlen(str)==1)
			    ans--;
			else
			{
		        judge=cool(str);
		        if (!judge)
		            ans--;
		    }
		}
		printf("Case %d: %d\n",sum,ans);
	}
	
	return 0;
 } 
