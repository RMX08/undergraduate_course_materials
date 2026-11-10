#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main()
{
    ofstream ofile;//定义输出流对象
    ifstream ifile;//定义输入流对象
    ofile.open("first.dat",ios::out | ios::binary);//以二进制的方式打开文件first.dat

    //从键盘输入5个整数到first.dat
    if (ofile)
    {
        for (int i=0;i<5;i++)
        {
            int temp;
            cin >> temp;
            ofile.write(reinterpret_cast<char*>(&temp),sizeof(int));//将temp中5个数写入文件输出流中，长度为int字节数
        }
        ofile.close();//因为后续要再同一流对象上打开另外的文件，因此需要使用close（）函数关闭当前文件

        //输出当前first.dat中的数字
        cout << "first.dat:" << endl;
        ifile.open("first.dat",ios::in | ios::binary);
        while (ifile.peek() != EOF)//peek（）为取文件当前指针位置，EOF为文件尾标识符
        {
            int temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(int));
            cout << temp <<" ";
        }
        ifile.close();
        cout << endl;
    }
    else
    {
        cout << "ERROR:Failed to open the file ""first.dat""." << endl;
    }

    //从键盘输入5个整数到second.dat
    ofile.open("second.dat",ios::out | ios::binary);
    if (ofile)//判定文件是否被打开，不是返回0
    {
        for (int i=0;i<5;i++)
        {
            int temp;
            cin >> temp;
            ofile.write(reinterpret_cast<char*>(&temp),sizeof(int));
        }
        ofile.close();

        //输出当前second.dat中的数字
        cout << "second.dat:" << endl;
        ifile.open("second.dat",ios::in | ios::binary);
        while (ifile.peek() != EOF)
        {
            int temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(int));
            cout << temp <<" ";
        }
        ifile.close();
        cout << endl;
    }
    else
    {
        cout << "ERROR:Failed to open the file ""second.dat""." << endl;
    }

        //从second.dat中读取5个整数，存放到first.dat原有数据的后面
    ofile.open("first.dat",ios::app | ios::binary);//打开一个输出文件first.dat，并仅用于在文件尾添加数据，不会删除原先内容
    ifile.open("second.dat",ios::in | ios::binary);//打开一个输入文件second.dat

    if (ofile && ifile)//判断两个文件是否被成功打开
    {
        while(ifile.peek() != EOF)
        {
            char temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(char));//从first.dat中读入数据
            ofile.write(reinterpret_cast<char *>(&temp),sizeof(char));//写入second.dat中
        }
        ifile.close();
        ofile.close();

        //输出当前first.dat中的数字
        cout << "first.dat:" << endl;
        ifile.open("first.dat",ios::in | ios::binary);
        while (ifile.peek()!=EOF)
        {
            int temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(int));
            cout << temp << " ";
        }
        ifile.close();
        cout << endl;
    }

    //从first.dat读取10个整数，升序排序后存放到second.dat
    int arr[11]={0};
    int i=0;
    ifile.open("first.dat",ios::in | ios::binary);
    ofile.open("second.dat",ios::out | ios::binary);//由于需要覆盖原来second.dat中原先的数字，因此可直接使用out，
    //隐含ios::trunc（若文件已存在则删除其中原先内容）

    if (ofile && ifile)
    {

        while (ifile.peek() != EOF)
        {
            int temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(int));
            arr[i++]=temp;//将从frist.dat中读取出来的数据存入数组中
        }
        ifile.close();

        sort(arr,arr+10);//通过sort（）对数组进行升序排序

        for (int j=0;j<i;j++)
        {
            ofile.write(reinterpret_cast<char *>(&arr[j]),sizeof(int));//将升序后数组中的数字输入second.dat中
        }
        ofile.close();

        //输出当前second.dat中的数字
        cout << "second.dat:" << endl;
        ifile.open("second.dat",ios::in | ios::binary);
        while (ifile.peek() != EOF)
        {
            int temp;
            ifile.read(reinterpret_cast<char *>(&temp),sizeof(int));
            cout << temp << " ";
        }
        ifile.close();
        cout << endl;
    }

    return 0;
}
