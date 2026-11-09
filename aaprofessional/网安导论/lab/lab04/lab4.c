#include<stdio.h>
#include<stdlib.h>

#ifdef MAX_N_MACRO
#define MAX_LIMIT MAX_N_MACRO
#else
#define MAX_LIMIT 1000 
#endif 

int Sum(int n)
{
	if (n == 1)
	    return n;
	else
	    return Sum(n-1) + n;
}

int main()
{
	int n, ans;
	scanf("%d", &n);
	
	if (n <= 0)
	{ 
	    printf("0\n");
	    return 0;
	}
	else if (n > MAX_LIMIT)
	{
		printf("-1\n");
		return 0;
	}
	
	ans = Sum(n);
	
	printf("%d", ans);
	return 0;
 } 
