#include<iostream>

typedef struct BiNode
{
    char data;
    struct BiNode *lchild;
    struct BiNode *rchild;
}BiNode,*BiTree;

void CreateBiTree(BiTree &T)
{
    char ch;
    scanf("%c", &ch);

    if (ch == '#') 
        T = NULL;
    else
    {
        T = (BiTree)malloc(sizeof(BiNode));
        T -> data = ch;
        CreateBiTree(T -> lchild);
        CreateBiTree(T -> rchild);
    }
}

void PreOrderTraverse(BiTree T)
{
    if (T)
    {
        printf("%c", T -> data);
        PreOrderTraverse(T -> lchild);
        PreOrderTraverse(T -> rchild);
    }
}

void InOrderTraverse(BiTree T)
{
    if (T)
    {
        InOrderTraverse(T -> lchild);
        printf("%c", T -> data);
        InOrderTraverse(T -> rchild);
    }
}

void PostOrderTraverse(BiTree T)
{
    if (T)
    {
        PostOrderTraverse(T -> lchild);
        PostOrderTraverse(T -> rchild);
        printf("%c", T -> data);
    }
}

int main()
{
    BiTree T;
    CreateBiTree(T);

    printf("先序遍历：");
    PreOrderTraverse(T);
    printf("\n");

    printf("中序遍历：");
    InOrderTraverse(T);
    printf("\n");

    printf("后序遍历：");
    PostOrderTraverse(T);
    printf("\n");
    
    return 0;
}