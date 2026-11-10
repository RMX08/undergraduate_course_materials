#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;
struct Student
{
	int num;
	char name[20];
	int score[3];//0代表语文，1代表数学，2代表英语 
 } ;
void save(struct Student stu1[53],int n)
{
 	FILE *fp;
 	if ((fp=fopen("student.dat","wb"))==NULL)
 	{
 		printf("cannot open file.\n");
 		exit(0);
	}
	for (int i=0;i<n;i++)
	{
		if (fwrite(&stu1[i],sizeof(struct Student),1,fp)!=1)
		    printf("file write error.\n");
	}
	fclose(fp);
} 
void sort(int n,int flag)
{
	struct Student stu1[53];
  	FILE *fp;
  	if ((fp=fopen("student.dat","rb"))==NULL)
    {
    	printf("cannot open file.\n");
    	exit(0);
	}
	for (int i=0;i<n;i++)
	{
		if (fread(&stu1[i],sizeof(struct Student),1,fp)!=1)
		{
			if (feof(fp))
			{
				fclose(fp);
				return;
			}
		}
	}
	for (int i=0;i<n-1;i++)
	    for (int j=0;j<n-i-1;j++)
	    {
	    	if (stu1[j].score[flag]>stu1[j+1].score[flag])
	    	{
	    		int tmp;
	    		char tmp1[100],tmp2[100];
	    		tmp=stu1[j].num;
	    		stu1[j].num=stu1[j+1].num;
	    		stu1[j+1].num=tmp;
	    		
	    		tmp=stu1[j].score[flag];
	    		stu1[j].score[flag]=stu1[j+1].score[flag];
	    		stu1[j+1].score[flag]=tmp;
	    		
	    		strcpy(tmp1,stu1[j].name);
	    		strcpy(tmp2,stu1[j+1].name);
	    		strcpy(stu1[j].name,"");
	    		strcpy(stu1[j+1].name,"");
	    		strcpy(stu1[j].name,tmp2);
	    		strcpy(stu1[j+1].name,tmp1);
			}
		}
	for (int i=0;i<n;i++)
	    printf("%d %s %d\n",stu1[i].num,stu1[i].name,stu1[i].score[flag]);
	fclose(fp);
}
 int main()
 {
 	struct Student stu[53];
 	int n;
 	printf("请输入学生个数：\n");
 	scanf("%d",&n);
 	printf("请输入学生信息（学号，姓名，语文、数学、英语成绩）:\n");
 	for (int i=0;i<n;i++)
 	    scanf("%d %s %d%d%d",&stu[i].num,&stu[i].name,&stu[i].score[0],&stu[i].score[1],&stu[i].score[2]);
 	save(stu,n);//将学生信息转存到磁盘文件
 	printf("语文成绩排序：\n");
	sort(n,0); //从磁盘中获取信息，排序，再输出 
	printf("\n数学成绩排序：\n");
	sort(n,1);
	printf("\n英语成绩排序：\n");
	sort(n,2);
	return 0;
 }
