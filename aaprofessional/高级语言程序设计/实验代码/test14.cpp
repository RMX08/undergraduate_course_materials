#include<stdio.h>
#include<stdlib.h>
#define n 10
#define len sizeof(struct Student)
struct Student
{
	int score;
	struct Student *next;
};
int main()
{
	struct Student stu[11]={{0},{98},{97},{92},{93},{90},{93},{92},{94},{93},{98}},*head,*p;
	head=&stu[1];
	p=head;
	for (int i=2;i<=n;i++)
	{
	    p->next=&stu[i];
	    p=p->next;
    }
	p->next=NULL;
	p=head;
	printf("该10个学生的成绩为：\n");
	while (p!=NULL)
	{
		printf("%d ",p->score);
		p=p->next;
	}
	printf("\n");
	
	
	struct Student add;
	int d1,d2;
	struct Student *p1=(struct Student*)malloc(len);
	printf("请输入想要添加的数据以及位置 x 2：\n");
	scanf("%d%d%d",&add.score,&d1,&d2);
	p1=&stu[d1];
	p1->next=&add;
	p1=p1->next;
	p1->next=&stu[d2];
	p=head;
	while (p!=NULL)
	{
		printf("%d ",p->score);
		p=p->next;
	}
	printf("\n");
	
	
	struct Student *p2,*p3,*p4;
	int d3;
	printf("请输入想要删除的数据的位置 x 1：\n");
	scanf("%d",&d3);
	p2=&stu[d3-1];//4 
	p3=p2->next;//5 
	p2->next=p2->next->next;//4-》6 
	free(p3);
	p=head;
	while (p!=NULL)
	{
		printf("%d ",p->score);
		p=p->next;
	}
	return 0;
 } 
