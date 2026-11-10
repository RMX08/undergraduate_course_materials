
#include "head.h"

double average(int n)
{
    if (n!=4)
    {
    	double sum=0;
    	int i;
        for (i=1;i<=10;i++)
            sum+=score[i][n];
        return sum*1.0/10;
    }
    else
    {
    	double sum=0;
    	int i,j;
        for (i=1;i<=10;i++)
            for (j=1;j<=3;j++)
                sum+=score[i][j];
        return sum*1.0/10;
    }
}

