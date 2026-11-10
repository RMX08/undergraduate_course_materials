#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

class Time
{
public:
    Time(int h,int m,int s);
    void setTime(int h,int m,int s);
    void showTime();
    ~Time(){}
private:
    int hour,minute,second;
};


#endif // CLOCK_H_INCLUDED

