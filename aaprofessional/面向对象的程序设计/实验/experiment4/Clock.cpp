#include "Clock.h"
#include <iostream>

Time::Time(int h,int m,int s):hour(h),minute(m),second(s){}

void Time::setTime(int h,int m,int s)
{
    hour=h;
    minute=m;
    second=s;
}

void Time::showTime()
{
    std::cout<<hour<<":"<<minute<<":"<<second<<std::endl;
}
