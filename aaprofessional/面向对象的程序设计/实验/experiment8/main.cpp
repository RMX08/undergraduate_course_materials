#include <iostream>
#include "LinkedList.h"

using namespace std;

int main()
{
    LinkedList<int> list;

    //输入10个整数依次向表头插入
    for (int i=0;i<10;i++)
    {
        int item;
        cin>>item;
        list.insertFront(item);
    }

    //输出链表
    cout<<"List:";
    list.reset();//将游标位置挪到头结点

    //输出各结点数据，直到链表尾
    while (!list.endOfList())
    {
        cout<<list.data()<<" ";
        list.next();
    }
    cout<<endl;

    //输出链表元素个数
    cout<<"Size:"<<list.getSize()<<endl<<endl;

    //输入需要删除的结点
    int key;
    cout<<"Please enter some integer needed to be deleted:";
    cin>>key;

    //查找并删除结点
    list.reset();
    while (!list.endOfList())
    {
        if (list.data()==key)
            list.deleteCurrent();
        list.next();
    }

    //输出当前链表
    cout<<"List:";
    list.reset();

    //输出各结点数据，直到链表尾
    while (!list.endOfList())
    {
        cout<<list.data()<<" ";
        list.next();
    }
    cout<<endl;

    //输出当前链表元素个数
    cout<<"Size:"<<list.getSize()<<endl<<endl;

    //删除头结点
    cout<<"Delete the first integer:"<<list.deleteFront()<<endl;

     //输出当前链表
    cout<<"List:";
    list.reset();

    //输出各结点数据，直到链表尾
    while (!list.endOfList())
    {
        cout<<list.data()<<" ";
        list.next();
    }
    cout<<endl;

    //输出当前链表元素个数
    cout<<"Size:"<<list.getSize()<<endl<<endl;

    //在表头添加一个新结点
    cout<<"Please enter the integer wanted to be added to the front:";
    cin>>key;
    list.insertFront(key);

    //输出当前链表
    cout<<"List:";
    list.reset();

    //输出各结点数据，直到链表尾
    while (!list.endOfList())
    {
        cout<<list.data()<<" ";
        list.next();
    }
    cout<<endl;

    //输出当前链表元素个数
    cout<<"Size:"<<list.getSize()<<endl<<endl;

    //输出当前结点
    cout<<"Current:"<<list.data()<<endl;

    //在当前结点之后插入新结点
    cout<<"Please enter the integer wanted to be added to the front of the current one:";
    cin>>key;
    list.insertAt(key);

    //输出当前链表
    cout<<"List:";
    list.reset();

    //输出各结点数据，直到链表尾
    while (!list.endOfList())
    {
        cout<<list.data()<<" ";
        list.next();
    }
    cout<<endl;

    //输出当前链表元素个数
    cout<<"Size:"<<list.getSize()<<endl<<endl;

    return 0;
}
