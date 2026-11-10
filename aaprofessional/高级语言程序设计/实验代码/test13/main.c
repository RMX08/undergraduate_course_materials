
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#include "head.h"
//#include "input.c"
//#include "average.c"

int main()
{
    printf("输入10个学生的三科成绩（语文、数学、英语）：\n");
    input();
    printf("语文平均成绩：  %.2lf\n",average(1));
    printf("数学平均成绩：  %.2lf\n",average(2));
    printf("英语平均成绩：  %.2lf\n",average(3));
    printf("三科平均成绩：  %.2lf\n",average(4));
    return 0;
}

