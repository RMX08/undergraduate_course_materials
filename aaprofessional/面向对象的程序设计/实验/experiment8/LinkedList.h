#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include "Node.h"

//front->a1->a2->...->rear

template <class T>
class LinkedList {
private:
    //数据成员
    Node<T> *front, *rear;//表头和表尾指针
    Node<T> *prePtr,*currPtr;//记录表当前遍历位置的指针，由插入和删除操作更新
    int size;//表中的元素个数
    int position;//当前元素在表中的位置序号(游标位置 currPtr)
    //函数成员
    //生成新结点，数据域为item，指针域为ptrNext
    Node<T> *newNode(const T & item,Node<T> *ptrNext=nullptr);

    //释放结点
    void freeNode(Node <T> *p);

    //将链表L复制到当前表（假设当前表为空）
    //被复制构造函数、operator=调用
    void copy(const LinkedList<T> & L);

public:
    LinkedList();//构造函数
    LinkedList(const LinkedList<T> & L);//复制构造函数
    ~LinkedList();//析构函数
    LinkedList<T> & operator=(const LinkedList<T> & L);//重载赋值运算符

    int getSize() const;//返回链表中元素个数
    bool isEmpty() const;//链表是否为空

    void reset(int pos=0);//初始化游标位置
    void next();//使游标移动到下一个结点
    bool endOfList() const;//游标是否到了链表尾
    int currentPosition() const;//返回游标当前位置

    void insertFront(const T & item);//在表头插入结点
    void insertRear(const T & item);//在表尾添加结点
    void insertAt(const T & item);//在当前结点之前插入结点
    void insertAfter(const T & item);//在当前结点之后插入结点

    T deleteFront();//删除头结点
    void deleteCurrent();//删除当前结点

    T & data();//返回对当前结点成员数据的引用
    const T & data() const;//返回对当前结点成员数据的常引用

    void clear();//清空链表：释放所有结点的内存空间，被析构函数、operator=调用
};


template <class T>
//生成新结点，数据域为item，指针域为ptrNext
Node<T> * LinkedList<T>::newNode(const T & item,Node<T> *ptrNext)
{
    Node<T> *tempPtr=new Node<T>(item,ptrNext);//为临时指针申请一个空间，其data为item，next为ptrNext
    return tempPtr;
}

template <class T>
//释放结点
void LinkedList<T>::freeNode(Node<T> *p)
{
    delete p;
}

template <class T>
//将链表L复制到当前表（假设当前表为空）
void LinkedList<T>::copy(const LinkedList<T> & L)
{
    Node<T> *temp=L.front,*ptr=front;//声明并定义赋值为头指针
    temp=temp -> nextNode();//将temp指向第一个结点
    while (temp!=L.rear)//遍历，除尾结点
    {
        ptr -> next=new Node<T>(temp -> data,nullptr);//为下一个结点申请空间
        ptr=ptr ->next;//当前结点指向下一个结点
        temp=temp -> next;
    }
    ptr -> next= rear;//指向尾结点
    size=L.getSize();
    position=L.currentPosition();
}

template <class T>
//构造函数
LinkedList<T>::LinkedList()
{
    rear=new Node<T>(0,nullptr);
    front=new Node<T>(0,rear);//=0，front->rear
    currPtr=rear;
    prePtr=front;
    size=0;//不计算front和rear
    position=0;//在front下一个元素视为0
    std::cout<<" Call Constructor"<<std::endl;
}

template <class T>
//复制构造函数
LinkedList<T>::LinkedList(const LinkedList<T> & L)
{
    rear=new Node<T>(0,nullptr);
    front=new Node<T>(0,rear);
    currPtr=rear;
    prePtr=front;
    size=0;
    position=0;
    copy(L);//复制链表
    std::cout<<"Call Copy constructor"<<std::endl;
}

template <class T>
//析构函数
LinkedList<T>::~LinkedList()
{
    clear();//清除链表
    delete front;//释放所申请的内存
    delete rear;
}

template <class T>
//重载复制运算符
LinkedList<T> & LinkedList<T>::operator=(const LinkedList<T> & L)
{
    clear();
    copy(L);
    std::cout<<"Call Operator Overloading"<<std::endl;
}

template <class T>
//返回元素个数
int LinkedList<T>::getSize() const
{
    return size;
}

template <class T>
//链表是否为空
bool LinkedList<T>::isEmpty() const
{
    return (size == 0);//当链表为空时返回1，不为空返回0
}

template <class T>
//初始化游标位置
void LinkedList<T>::reset(int pos)
{
    if (pos>size)//判断游标是否大于总长度
    {
        std::cout<<"越界，无法访问"<<std::endl;
        return;
    }
    prePtr=front;
    currPtr=front -> nextNode();
    for (int i=0;i<pos;i++)//遍历，将currPtr指向游标位置
    {
        prePtr=currPtr;
        currPtr=currPtr -> nextNode();
    }
    position=pos;
}

template <class T>
//使游标移动到下一个结点
void LinkedList<T>::next()
{
    if  (currPtr == rear)
    {
        std::cout<<"已经到达链表尾，无法移动"<<std::endl;
        return ;
    }
    prePtr=currPtr;
    currPtr=currPtr -> nextNode();
    position++;
}

template <class T>
//游标是否到了链表尾
bool LinkedList<T>::endOfList() const
{
    return (currPtr == rear);//如果已经到达链表尾返回1，没有返回0
}

template <class T>
//返回游标当前位置
int LinkedList<T>::currentPosition() const
{
    return position;
}

template <class T>
//在表头插入结点
void LinkedList<T>::insertFront(const T & item)
{
    Node<T> *temp= new Node<T>(item,front -> nextNode());//temp->data=item,temp->next=front->next
    front=new Node<T>(0,temp);//为头指针申请新的内存空间，并指向当前插入的点
    size++;
    position++;
}

template <class T>
//在表尾添加结点
void LinkedList<T>::insertRear(const T & item)
{
    Node<T> *p1=front,*p2=front ->nextNode();
    while (p2!=rear)//遍历，最终p1指向最后一个结点，p2指向rear
    {
        p1=p2;
        p2=p2 -> nextNode();
    }
    Node<T> *temp= new Node<T>(item,rear);
    p1 -> insertAfter(temp);//在p1之后插入一个同类结点temp，指向rear
    size++;
}

template <class T>
//在当前结点之前插入结点
void LinkedList<T>::insertAt(const T & item)
{
    Node<T> *temp= new Node<T>(item,currPtr);//声明一个临时指针指向当前结点
    prePtr -> insertAfter(temp);//在前一个结点之后插入该新结点
    size++;
}

template <class T>
//在当前结点之后插入结点
void LinkedList<T>::insertAfter(const T & item)
{
    Node<T> *temp= new Node<T>(item,currPtr -> nextNode());//声明一个临时指针指向当前结点后一个结点
    currPtr -> insertAfter(temp);//在当前结点后插入该新结点
    size++;
}

template <class T>
//删除头结点
T LinkedList<T>::deleteFront()
{
    Node<T> *temp=front -> nextNode();//声明一个临时指针指向头结点
    temp=temp -> nextNode();//将临时指针指向第二个结点
    delete front;//释放原本头结点内存
    front= new Node<T>(0,temp);//为头指针申请一个新的内存，并指向第二个结点
    size--;
    return temp->data;//返回当前头指针的数据域
}

template <class T>
//删除当前结点
void LinkedList<T>::deleteCurrent()
{
    Node<T> *temp=prePtr -> deleteAfter();//声明一个临时指针指向需要删除的节点
    currPtr=temp -> nextNode();
    delete temp;
    size--;
}

template <class T>
//返回对当前结点成员数据的引用
T & LinkedList<T>::data()
{
    return currPtr -> data;
}

template <class T>
////返回对当前结点成员数据的引用
const T & LinkedList<T>::data()const
{
    return currPtr;
}

template <class T>
//清空链表：释放所有结点的内存空间
void LinkedList<T>::clear()
{
    Node<T> *temp=front -> nextNode();
    while (temp!=rear)//从头结点开始遍历，
    {
        front=temp;
        delete temp;
        temp=front -> nextNode();
    }
    size=0;
    position=0;
}

#endif // LINKEDLIST_H_INCLUDED
