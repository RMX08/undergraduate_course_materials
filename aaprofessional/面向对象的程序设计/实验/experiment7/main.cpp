#include <iostream>
#include <cmath>

using namespace std;

const double PI=3.14159;

class Shape{
public:
    virtual void display()=0;
    virtual void area()=0;
    virtual void perimeter()=0;
};

class Square:public Shape{
public:
    Square (double aa=0,double bb=0):a(aa),b(bb){}
    void display();
    void area()
    {
        cout<<"Area:"<<a*b<<endl;
    }
    void perimeter()
    {
        cout<<"Perimeter:"<<2*(a+b)<<endl;
    }
private:
    double a;
    double b;
};
void Square::display()
{
    cout<<"--Square--"<<endl;
}

class Circle:public Shape{
public:
    Circle (double rr=0):r(rr){}
    void display();
    void area()
    {
        cout<<"Area:"<<PI*r*r<<endl;
    }
    void perimeter()
    {
        cout<<"Perimeter:"<<2*PI*r<<endl;
    }
private:
    double r;
};
void Circle::display()
{
    cout<<"--Circle--"<<endl;
}

class Rectangle:public Shape{
public:
    Rectangle (double aa=0,double bb=0,double cc=0):a(aa),b(bb),c(cc){}
    void display()
    {
        cout<<"--Rectangle--"<<endl;
    }
    void area();
    void perimeter()
    {
        cout<<"Perimeter:"<<a+b+c<<endl;
    }
private:
    double a,b,c;
    double p;
};
void Rectangle::area()
{
    p=(a+b+c)*1.0/2;
    cout<<"Area:"<<sqrt(p*(p-a)*(p-b)*(p-c))<<endl;
}

int main()
{
    Square square(3,4);
    Circle circle(1);
    Rectangle rectangle(5,5,6);
    Shape *ptr[3];

    ptr[0]=&square;
    ptr[1]=&circle;
    ptr[2]=&rectangle;

    for (int i=0;i<3;i++)
    {
        ptr[i] -> display();
        ptr[i] -> area();
        ptr[i] -> perimeter();
    }

    return 0;
}
