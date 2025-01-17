#include <iostream>
#include <thread>

// 入口函数
void f()
{
    std::cout << "Hello, concurent world!" << std::endl;
}

int main()
{
    std::thread t{f}; // 构造std::thread对象，启动线程
    t.join();         // 加入式，结束线程
}