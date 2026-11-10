#include<cstdio>
void sort(int b[10])
{
	for (int i=0;i<9;i++)
	    for (int j=0;j<9-i;j++)
	    {
	    	if (b[j]>b[j+1])
	    	{
	    		int tmp;
	    		tmp=b[j];
	    		b[j]=b[j+1];
	    		b[j+1]=tmp;
			}
		}
	for (int i=0;i<10;i++)
	    printf("%d ",b[i]);
	printf("\n");
	return ;
}
int main()
{
	int a[3][10];
	for (int i=0;i<3;i++)
	    for (int j=0;j<10;j++)
	        scanf("%d",&a[i][j]);
	for (int i=0;i<3;i++)
		sort(a[i]);
	return 0;
 } 
