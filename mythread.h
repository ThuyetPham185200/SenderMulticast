#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<thread>
#include<iostream>
class MyThread
{
public:
    MyThread(std::thread t);
    ~MyThread();
    MyThread(MyThread& source);
    MyThread& operator=(MyThread& source);
    MyThread&& operator =(MyThread&& source);
private:
    std::thread t_;
};

#endif // MYTHREAD_H
