#include "Clock.h"
#include <iostream>

using namespace std;

int main()
{
    Time clock(0,0,0);
    cout<<"set time and output"<<endl;

    clock.setTime(8,30,30);
    clock.showTime();

    return 0;
}
