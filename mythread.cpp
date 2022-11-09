#include "mythread.h"

MyThread::MyThread(std::thread t) : t_(std::move(t)) {}
///////////////////////////////////////////////////
MyThread::~MyThread()
{
    if (this->t_.joinable())
    {
        this->t_.join();
    }
}
///////////////////////////////////////////////////
MyThread::MyThread(MyThread& source) {}
///////////////////////////////////////////////////
MyThread& MyThread::operator = (MyThread& source)
{
    this->t_ = std::move(source.t_);
}
///////////////////////////////////////////////////
MyThread&& MyThread::operator = (MyThread&& source)
{
    this->t_ = std::move(source.t_);
}
