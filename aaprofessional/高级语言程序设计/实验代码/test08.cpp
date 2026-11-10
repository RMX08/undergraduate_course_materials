#include<cstdio>
#include<iostream>
using namespace std;
void TEST01()
{
	printf("TEST 08-1\n");
	int n,a[100000]={0},sum=1;
	scanf("%d",&n);
	for (int i=0;i<n;i++)
	    scanf("%d",&a[i]);
	for (int i=1;i<n;i++)
	{
		//printf("%d ",a[i]);
		if (a[i]!=a[i-1])
			sum++;
	}
	printf("%d",sum);
	return; 
}
void TEST02()
{
	int n,tmp,a[10000]={0},min=999999;
	scanf("%d",&n);
	for (int i=0;i<n;i++)
	    scanf("%d",&a[i]);
	for (int i=0;i<n-1;i++)
	  for (int j=0;j<n-1-i;j++)
	  {
	  	//cout<<i<<" "<<j<<endl;
	  	if (a[j]>a[j+1])
	  	{
	  		tmp=a[j];
	  		a[j]=a[j+1];
	  		a[j+1]=tmp;
	    }
	  }
//	for (int i=0;i<n;i++)
//	    cout<<a[i]<<" ";
//	cout<<endl;
	for (int i=1;i<n;i++)
		if (a[i]-a[i-1]<min)
		    min=a[i]-a[i-1];
	printf("%d",min);
	return;
}
void TEST03()
{
	int n,sum=0;
	scanf("%d",&n);
	n=n/10;
	if (n/5!=0)
	{
		sum+=n/5*7;
		n=n%5;
	}
	if (n/3!=0)
	{
		sum+=n/3*4;
		n=n%3;
	}
	if (n!=0)
	    sum+=n;
	printf("%d",sum);
	return;
}
int main()
{
	TEST01();
	TEST02();
	TEST03();
	return 0;
 } 
 /*120
 yi 4+4+4+4=16
 er 7+7+2=16
 150
 yi 4+4+4+4+4=20
 er 7+7+7=21
 */
