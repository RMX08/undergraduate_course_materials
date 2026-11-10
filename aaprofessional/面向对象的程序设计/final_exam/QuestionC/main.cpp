#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int AX=0,BX=0,CX=0,DX=0;

int judge(string s)
{
    if (s == "AX")
        return AX;
    else if (s=="BX")
        return BX;
    else if (s=="CX")
        return CX;
    else
        return DX;
}

void in(string s,int num)
{
    if (s == "AX")
        AX=num;
    else if (s=="BX")
        BX=num;
    else if (s=="CX")
        CX=num;
    else
        DX=num;

}

void out(string s)
{
    cout<<judge(s);
}

void add(string s1,string s2)
{
    if (s1 == "AX")
        AX+=judge(s2);
    else if (s1=="BX")
        BX+=judge(s2);
    else if (s1=="CX")
        CX+=judge(s2);
    else
        DX+=judge(s2);
}

void mul(string s1,string s2)
{
    if (s1 == "AX")
        AX*=judge(s2);
    else if (s1=="BX")
        BX*=judge(s2);
    else if (s1=="CX")
        CX*=judge(s2);
    else
        DX*=judge(s2);
}

void div(string s1,string s2)
{
    if (s1 == "AX")
        AX/=judge(s2);
    else if (s1=="BX")
        BX/=judge(s2);
    else if (s1=="CX")
        CX/=judge(s2);
    else
        DX/=judge(s2);
}

int main()
{
    ifstream ifile;
    ifile.open("instruction.txt",ios_base::in);

    if (ifile)
    {
        while (ifile.peek() != EOF)
        {
            string instruction;
            string str1,str2;
            int num;

            ifile>>instruction;

            char ch;
            for (int i=0;i<2;i++)
            {
                ifile>>ch;
                str1+=ch;
            }
            ifile>>ch;
            if (instruction == "IN" )
                ifile>>num;
            else if (instruction != "OUT")
            {
                ifile>>ch;
                str2+=ch;
                ifile>>ch;
                str2+=ch;
            }

            if (instruction == "IN")
                in(str1,num);
            else if (instruction == "OUT")
                out(str1);
            else if (instruction == "ADD")
                add(str1,str2);
            else if (instruction == "MUL")
                mul(str1,str2);
            else if (instruction == "DIV")
                div(str1,str2);

        }
    }
    else
        cout<<"ERROR"<<endl;

    return 0;

}
