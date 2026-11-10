#include<stdio.h>
#include<iostream>
#include<string.h>
using namespace std;
struct Student
{
	int number;
	string name;
	int score[3];//英语，数学，语文 
}; 
void sort(struct Student stu1[102],int n,int flag)
{
	for (int i=1;i<n;i++)
	    for (int j=1;j<=n-i;j++)
	    {
	    	//printf("%d %d\n",stu1[j].score[flag],stu1[j+1].score[flag]);
	    	if (stu1[j].score[flag]>stu1[j+1].score[flag])
	        {
	        	int tmp;
	        	char tmp1[100],tmp2[100];
	        	tmp=stu1[j].score[flag];
	        	stu1[j].score[flag]=stu1[j+1].score[flag];
	        	stu1[j+1].score[flag]=tmp;
	        	tmp=stu1[j].number;
	        	stu1[j].number=stu1[j+1].number;
	        	stu1[j+1].number=tmp;
	        	strcpy(tmp1,stu1[j].name.c_str());
	        	strcpy(tmp2,stu1[j+1].name.c_str());
	        	stu1[j].name="\0";
	        	stu1[j+1].name="\0";
	        	strcpy((char*)stu1[j].name.c_str(),tmp2);
	        	strcpy((char*)stu1[j+1].name.c_str(),tmp1);
			}
		}
	for (int i=1;i<=n;i++)
	    printf("%d %s\n",stu1[i].number,stu1[i].name.c_str());
	return ;
}
int main()
{
	struct Student stu[102];
	int n,flag;
	printf("请输入学生总人数：\n");
	scanf("%d",&n);
	printf("请输入学生信息：（学号，姓名，英语成绩，数学成绩，语文成绩）\n");
	for (int i=1;i<=n;i++)
	{
		scanf("%d",&stu[i].number);
		scanf("%s",stu[i].name.c_str());
		scanf("%d%d%d",&stu[i].score[0],&stu[i].score[1],&stu[i].score[2]);
	}
	printf("\n英语成绩排名\n");
	flag=0;
	sort(stu,n,flag);
	printf("\n数学成绩排名\n");
	flag=1;
	sort(stu,n,flag);
	printf("\n语文成绩排名\n");
	flag=2;
	sort(stu,n,flag);
	return 0;
 } 
