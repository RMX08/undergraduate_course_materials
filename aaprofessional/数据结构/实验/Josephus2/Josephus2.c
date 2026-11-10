#include<stdio.h>
#include<stdlib.h>

typedef struct node 
{
    int data;
    struct node *next;
    struct node *prior;
}LinkNode, *LinkList;

LinkList Init_Ring(LinkList R,int n);

LinkList Delete_Death(LinkList R,int n, int m1, int m2, int target);

void Print_Live(LinkList R, int target);

int main()
{
    int n,m1,m2,target;  //n总人数，m1正向间隔数，m2反向间隔数，target剩余人数
    LinkList R;

    scanf("%d %d %d %d",&n,&m1,&m2,&target);

    R = Init_Ring(R,n);

    printf("死者顺序：");
    R = Delete_Death(R,n,m1,m2,target);

    printf("\n生者：");
    Print_Live(R,target);

    return 0;
}

LinkList Init_Ring(LinkList R, int n)//不同于单向，head指向第一个元素，不存在头结点
{
    LinkList temp,head,tail;
    R = (LinkList)malloc(sizeof(LinkList));
    head = R;
    head -> data = 1;//第一个元素
    head -> next = NULL;
    head -> prior = NULL;
    
    tail = head;

    for (int i = 2; i <= n; i ++)
    {
        temp = (LinkList)malloc(sizeof(LinkList));
        temp -> data = i;
        tail -> next = temp;
        temp -> prior = tail;
        tail = temp;
    }
    //使双向链表成为循环链表
    tail -> next = head;
    head -> prior = tail;

    return head;
}

LinkList Delete_Death(LinkList headR, int n, int m1, int m2, int target)
{
    LinkList temp,p = headR;
    int len = n;//1正向 -1反向

    while(len != target)
    {
        //正向
        temp = (LinkList)malloc(sizeof(LinkList));
        for (int i = 1; i < m1-1; i ++)//指向被删除的前一位
            p = p -> next;
        temp = p -> next;//被删除

        if (temp == headR)//防止头结点被删除
            headR = temp -> next;

        p -> next = temp -> next;
        temp -> next -> prior = p;
        printf("%d ",temp -> data);
        free(temp);
        p = p -> next;
        len --;

        if (len == target) break;

        //反向
        temp = (LinkList)malloc(sizeof(LinkList));
        for (int i = 1; i < m2-1; i ++)//指向被删除的后一位
            p = p -> prior;
        temp = p -> prior;

        if (temp == headR)//防止头结点被删除
            headR = temp -> next;

        p -> prior = temp -> prior;
        temp -> prior -> next = p;
        printf("%d ",temp -> data);
        free(temp);
        p = p -> prior;
        len --;
    }
    return headR;
}

void Print_Live(LinkList R, int n)
{
    LinkList p = R;
    while(n --)
    {
        printf("%d ",p -> data);
        p = p -> next;
    }
}