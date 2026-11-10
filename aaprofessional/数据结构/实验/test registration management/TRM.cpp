# include <iostream>
# include <vector>
# include <string>
using namespace std;

struct Student
{
    string id;
    string name;
    string subject;
};

void Add_Student(vector<Student>& students, string id, string name, string subject);

void Print_Student(vector<Student>& students);

void Query_Student(vector<Student>& students,string subject);

void Modify_Student(vector<Student>& students,string id,string subject);

void Delete_Student(vector<Student>& students,string id);

void Sum_Student(vector<Student>& students);

int main()
{
    vector<Student> students;
    int option,n;//操作方式，学生人数
    string id, name, subject;
    
    
    cin >> option;
        if (option == 1)//报名
        {
            cin >> n;
            
            for (int i = 0; i < n; i ++)
            {
                cin >> id >> name;
                std::getline(cin >> std::ws, subject); 
                //cin默认空格、制表符、换行停止，防止科目名字中的空格影响输入，std::ws 用于忽略输入流中的空白字符，确保 getline 能正确读取包含空格的科目。

                Add_Student(students,id,name,subject);
            }
        }
    cin >> option;

        if (option == 2)//输出所有学生
        {
            Print_Student(students);
        }
    cin >> option;
        if (option == 3)//查询对应科目下学生名单
        {
            std::getline(cin >> std::ws, subject); 
            
            Query_Student(students,subject);
        }
    cin >> option;
        if (option == 4)//修改某学生的考试科目
        {
            cin >> id >> subject;
            
            Modify_Student(students,id,subject);
            Print_Student(students);
        }
    cin >> option;
        if (option == 5)//删除报名信息
        {
            cin >> id;
            
            Delete_Student(students,id);
            Print_Student(students);
        }
    cin >> option;
        if (option == 6)//统计各科目下学生人数
        {
            Sum_Student(students);
        }
        
    

    return 0;
}

void Add_Student(vector<Student>& students,string id,string name,string subject)
{
    Student newstudent = {id,name,subject};
    students.push_back(newstudent);//尾插
}

void Print_Student(vector<Student>& students)
{
    for (const auto& student : students)
        cout << student.id << " " << student.name << " " << student.subject << endl;
}

void Query_Student(vector<Student>& students,string subject)
{
    for (const auto& student : students)
    {
        if (student.subject == subject)
            cout << student.id << " " << student.name << " " << student.subject << endl;
    }
}

void Modify_Student(vector<Student>& students,string id,string subject)
{
    for (auto& student : students)
    {
        if (student.id == id)
        {
            student.subject = subject;
            break;
        }
    }
}

void Delete_Student(vector<Student>& students,string id)
{
    for (auto it = students.begin(); it != students.end(); ++ it)
    {
        if (it -> id == id)
        {
            students.erase(it);
            break;
        }
    }
}

void Sum_Student(vector<Student>& students)
{
    vector<pair<string, int>> SubjectSum;//新容器，科目，人数
    for (const auto& student : students)
    {
        bool flag = false;
        for (auto& count : SubjectSum)
        {
            if (count.first == student.subject)
            {
                count.second ++;
                flag = true;
                break;
            }
        }

        if (!flag)//当前为该科目第一次遇见
            SubjectSum.push_back({student.subject,1});
    }

    for (const auto& count : SubjectSum)
        cout << count.first << " " << count.second << endl;
}