#include <iostream>

using namespace std;

class Person{
public:
    string name;
    Person(string n):name(n){
        cout<<"Person "<<name<<" constructed."<<endl;
    }
};

class Waiter:virtual public Person{
public:
    string name;
    Waiter(string n):Person(n),name(n){
        cout<<"Waiter "<<name<<" constructed."<<endl;}
};

class Singer:virtual public Person{
public:
    string name;
    Singer(string n):Person(n),name(n){
        cout<<"Singer "<<name<<" constructed."<<endl;
    }
};

class SingingWaiter:public Waiter,public Singer{
public:
    string name;
    SingingWaiter(string n):Person(n),Waiter(n),Singer(n),name(n){
        cout<<"SingingWaiter "<<name<<" constructed."<<endl;
    }
};

int main()
{
    cout << "Please give the name of a singing waiter:";
    string n;
    cin>>n;

    SingingWaiter swaiter(n);

    return 0;
}
