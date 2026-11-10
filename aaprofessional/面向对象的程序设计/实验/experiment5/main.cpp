#include<iostream>

using namespace std;

class Person {
public:
    Person(int age=0):age0(age)
    {
        cout<<"Constructing Person "<<age<<endl;
    }

    void print_age()
    {
        cout<<"ÄêÁä: "<<age0<<"  Person"<<endl;
    }
    ~Person()
    {
        cout<<"Destructing Person "<<endl;
    }
    int age0;
};

class Student:virtual public Person {
public:
    Student(long long n):num(n)
    {
        cout<<"Constructing Student "<<num<<endl;
    }

    ~Student()
    {
        cout<<"Destructing Student "<<endl;
    }
private:
    long long num;
};

class Employee:virtual public Person {
public:
    Employee(string str):str0(str)
    {
        cout<<"Constructing Employee "<<str0<<endl;
    }

    ~Employee()
    {
        cout<<"Destructing Employee "<<endl;
    }
private:
    string str0;
};

class PartTime_Graduate_Student:public Employee,public Student{
public:
    PartTime_Graduate_Student(int age,string str,long long num):Person(age),Employee(str),Student(num){}
    void print_age()
    {
        cout<<"ÄêÁä£º"<<age0<<" PartTime_Graduate_Student"<<endl;
    }
    ~PartTime_Graduate_Student()
    {
        cout<<"Destructing PartTime_Graduate_Student"<<endl;
    }
};

int main()
{
    PartTime_Graduate_Student stu(27,"RMX",21080012078);

    cout<<stu.age0<<endl;

    stu.print_age();
    stu.Person::print_age();

    return 0;
}

