#include<iostream>

using namespace std;

char point[100];
int n = 0;

void GetRating()
{
    char ch;
    while (cin >> ch,ch!='0')
    {
        if (ch=='E' || ch=='G' || ch=='A' || ch=='P')
            point[n++] = ch;
        else
            printf("Input Error!!!\nPlease input again.\n");
    }
}

void PrintRating()
{
    for (int i=0; i<n; i++)
    {
        switch(point[i])
        {
        case 'E':
            cout << "优" << endl;
            break;
        case 'G':
            cout << "良" << endl;
            break;
        case 'A':
            cout << "中" << endl;
            break;
        default:
            cout << "差" << endl;
        }
    }
}

int max2(int a,int b)
{
    return a>b?a:b;
}

int max3(int a=2,int b=3,int c=10);

int main()
{
    //Experiment1
    cout << "EX1:" << endl;
    cout << "Input Point:" << endl;
    GetRating();

    cout << endl << "Rating:" << endl;
    PrintRating();


    //Experiment2
    cout << endl << "EX2 :" << endl;
    cout << "1、比较a和b，输入a和b" << endl << "2、比较a、b、c，默认带参数 2 3 10"<< endl <<endl;
    cout << "请输入 a b" << endl;
    int a,b;
    cin >> a >> b;
    cout << max2(a,b) << endl;

    cout << max3() << endl;

    return 0;
}

int max3(int a,int b,int c)
{
    int M;

    M = a>b?a:b;
    M = M>c?M:c;

    return M;
}
