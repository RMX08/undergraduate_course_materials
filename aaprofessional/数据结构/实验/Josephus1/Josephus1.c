# include<stdio.h>
# include<stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
}LinkNode, *LinkList;

LinkList Init_Ring(LinkList R, int n);  //初始化链表

LinkList Delete_Death(LinkList R, int n, int m, int target);//删除死者并按照顺序输出死者编号

void Print_Live(LinkList R,int target);  //输出当前链表剩余编号

int main()
{
    int n,m,target;  //n总人数，m间隔数，target剩余人数
    LinkList R;

    scanf("%d %d %d",&n,&m,&target);

    R = Init_Ring(R,n);

    printf("死者顺序：");
    R = Delete_Death(R,n,m,target);

    printf("\n生者：");
    Print_Live(R,target);

    return 0;

}

LinkList Init_Ring(LinkList R, int n)
{
    LinkList tail,head,temp;
    R = (LinkList)malloc(sizeof(LinkList));  //创建头结点
    R -> next = NULL;   
    head = R;
    tail = R;

    for (int i = 1; i <= n; i ++) //建立1~n的环形链表
    {
        temp = (LinkList)malloc(sizeof(LinkList));
        temp -> data = i;
        tail -> next = temp;
        temp -> next = head -> next;
        tail = temp;
    }

    return head;
}

LinkList Delete_Death(LinkList R, int n, int m, int target)
{
    int len = n;  //len剩余人数
    LinkList p = R;//指向头结点
    LinkList temp;
    
    while(len != target)
    {
        for (int i = 1; i < m; i ++) //指向被删除的前一位
            p = p -> next;
        
        temp = p -> next;

        if (temp -> data == R -> next -> data)//如果要删除的元素为头结点后第一个元素，防止头结点丢失
        {
            R -> next = temp -> next; //可以确保头结点后第一个元素存在并为当前最小编号
        }

        p -> next = temp -> next;
        printf("%d ", temp -> data);//输出被删除的死者

        free(temp);
        len --;  
    }
    return R;
}

void Print_Live(LinkList R, int n)
{
    LinkList temp = R -> next;
    while(n --)
    {
        printf("%d ", temp -> data);
        temp = temp -> next;
    }
}