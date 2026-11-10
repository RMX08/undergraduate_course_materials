#include<stdio.h>
int main()
{
	int n;
	int a[502][502];
	scanf("%d",&n);
	for (int i=0;i<n;i++)
	  for (int j=0;j<n;j++)
	  	scanf("%d",&a[i][j]);
	for (int i=0;i<n;i++)//前半部分 
	{
		if (i%2==1)
		{
		    for (int j=0;j<=i;j++)
		  	    printf("%d ",a[j][i-j]);
		}
		else
		{
			for (int j=0;j<=i;j++)
		  	    printf("%d ",a[i-j][j]);
		}
	}
	if (n%2==1)
	{
	    for (int i=1;i<n;i++)//后半部分 
	    {
		    if (i%2==1)
		    {
		    	int m=1;
			    for (int j=i;j<n;j++)
			        printf("%d ",a[j][n-(m++)]);
		    }
		    else
		    {
		    	int m=1;
			    for (int j=i;j<n;j++)
			        printf("%d ",a[n-(m++)][j]);
		    }
	    }
	}
	else
	{
		for (int i=1;i<n;i++)//后半部分 
	    {
		    if (i%2==1)
		    {
		    	int m=1;
			    for (int j=i;j<n;j++)
			        printf("%d ",a[n-(m++)][j]);
		    }
		    else
		    {
		    	int m=1;
			    for (int j=i;j<n;j++)
			        printf("%d ",a[j][n-(m++)]);
		    }
	    }
	}
	return 0;
 } 
 /*
 0 0 0
 
 0 1 1
 1 0
 
 2 0 2
 1 1
 0 2
 
 0 3 3
 1 2
 2 1
 3 0 

4 0 4
3 1
2 2
1 3
0 4

1(j) 4(n-j) 1
2(j+1) 3(n-(j+1))
3 2
4 1

j=i i=2
4(n-j+1) 2(j)2 
3(n-(j+1)+1) 3(j+1)
2 4

3 4 3
4 3
 
4 4 4

3(4-1) 1 1
2(4-2) 2
1(4-3) 3

2 3 2
3 2

3(4-1) 3 3

5(n-1) 1(j) 1
4 2
3 3
2 4
1(n-4) 5

2(j) 5 2
3 4
4 3
5 2

5 3 3
4 4
3 5

4 5 4
5 4

5 5
*/

