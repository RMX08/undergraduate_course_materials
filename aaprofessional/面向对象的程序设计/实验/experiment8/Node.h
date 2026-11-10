#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

template <class T>
class Node{
private:
    Node<T> *next;//指向后继结点的指针
public:
    T data;//数据域

    Node(const T & data,Node<T> *next=0);//构造函数
    void insertAfter(Node<T> *p);//在本结点之后插入一个同类结点p
    Node<T> *deleteAfter();//删除本结点的后继结点，并返回其地址
    Node<T> *nextNode();//获取后继结点的地址
    const Node<T> *nextNode() const;//获取后继结点的地址
};


//构造函数，初始化数据和指针成员
template <class T>
Node<T>::Node(const T & data,Node<T> *next/*=0*/):data(data),next(next){}//对于默认值的声明，只要在定义的时候声明就行 连续声明两次会报错redeclaration

//获取后继结点的地址
template <class T>
Node<T> * Node<T>::nextNode()
{
    return next;
}

//获取后继结点的地址
template <class T>
const Node<T> * Node<T>::nextNode() const
{
    return next;
}

//在本结点之后插入一个同类结点p
template <class T>
void Node<T>::insertAfter(Node<T> *p)
{
    p -> next=this -> next;//p指针指向当前结点后一个结点
    this -> next=p;//当前结点指向p结点
}

//删除本结点的后继结点，并返回其地址
template <class T>
Node<T> * Node<T>::deleteAfter()
{
    Node<T> * tempPtr = this -> next;//临时指针指向想要删除的结点
    if (this -> next=0)//如果当前结点没有后继结点，返回空指针
        return 0;
    this -> next=tempPtr -> next;//使当前结点指针指向Temp的后继结点，从而达到删除一个结点的操作
    return tempPtr;//返回删除结点的地址
}
#endif // NODE_H_INCLUDED
