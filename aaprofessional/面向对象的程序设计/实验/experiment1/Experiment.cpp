#include<iostream>

using namespace std;

int main()
{
    cout<<"请选择图形类型 （输入数字）: "<<endl;
    cout<<"1、圆形"<<endl;
    cout<<"2、长方形"<<endl;
    cout<<"3、正方形"<<endl;

    int type;
    cin>>type;

    if (type==1)
    {
        cout<<"请输入圆形半径 : "<<endl;

        double R;
        cin>>R;

        cout<<"圆的面积为 "<<3.14*R*R<<" （Π取3.14）"<<endl;
    }
    else if (type==2)
    {
        cout<<"请输入长方形的长和宽 ： "<<endl;

        double a,b;
        cin>>a>>b;

        cout<<"长方形面积为 "<<a*b<<endl;
    }
    else
    {
        cout<<"请输入正方形的边长 : "<<endl;

        double a;
        cin>>a;

        cout<<"正方形的面积为 "<<a*a<<endl;
    }

    return 0;
}

