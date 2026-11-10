#include <iostream>

using namespace std;

class Square_Matrix{
public:
    Square_Matrix(int num=0):n(num)
    {
        cout << "n= " << n <<endl;
    }
    friend istream & operator>> (istream &in,Square_Matrix &matrix);
    friend ostream & operator<< (ostream &out,const Square_Matrix &matrix);
    Square_Matrix operator+ (const Square_Matrix &matrix0);
    Square_Matrix & operator= (const Square_Matrix &matrix);
    void intmatrix(int n)
    {
        for (int i=0;i<n;i++)
           arrB[i][i]=1;
    }

private:
    int n;
    int arr[6][6]={0};
    int arrB[6][6]={0};
};

istream &operator>> (istream &in,Square_Matrix &matrix)
{
    for (int i=0;i<matrix.n;i++)
      for (int j=0;j<matrix.n;j++)
        in >> matrix.arr[i][j];
    return in;
}

ostream &operator<< (ostream &out,const Square_Matrix &matrix)
{
    for (int i=0;i<matrix.n;i++)
    {
        for (int j=0;j<matrix.n;j++)
            out << matrix.arr[i][j];
        out << endl;
    }
    return out;
}

Square_Matrix Square_Matrix::operator+ (const Square_Matrix &matrix0)
{
    Square_Matrix matrix(5);
    for (int i=0;i<matrix.n;i++)
        for (int j=0;j<matrix.n;j++)
        matrix.arr[i][j]=this->arr[i][j]+matrix0.arrB[i][j];
    return matrix;
}

Square_Matrix & Square_Matrix::operator= (const Square_Matrix &matrix)
{
    for (int i=0;i<matrix.n;i++)
        for (int j=0;j<matrix.n;j++)
            this -> arr[i][j] = matrix.arr[i][j];
}

int main()
{
    Square_Matrix A(5),B(5),C(5);
    cout << "input a matirx A" << endl;//输入矩阵
    cin >> A;
    cout << "output the matrix A" << endl << A << endl;//输出矩阵

    B.intmatrix(5);//生成一个单位矩阵

    C=A+B;

    cout << "output A+B" << endl << C << endl;

    return 0;
}
