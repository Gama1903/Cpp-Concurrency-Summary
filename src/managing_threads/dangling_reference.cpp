#include <iostream>
#include <thread>

struct Foo
{
    int &x_;
    Foo(int &x) : x_(x) {}
    void operator()()
    {
        for (size_t i = 0; i < 1000000; ++i)
        {
            x_++; // 悬空引用
        }
    }
};

int main()
{
    int x = 0; // main函数结束，x的内存会被回收，但Foo对象还在使用x的引用
    std::thread t{Foo(x)};
    t.detach();
}