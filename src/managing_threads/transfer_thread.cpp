#include <iostream>
#include <thread>
#include <cassert>

void f() {}
void g() {}

int main()
{
    std::thread t1{f};              // 创建线程t1
    assert(t1.joinable());          // 通过，t1拥有资源
    std::thread t2 = std::move(t1); // 对象显式移动
    assert(!t1.joinable());         // 通过，t1已经不再拥有资源
    assert(t2.joinable());          // 通过，t2拥有资源

    t1 = std::thread{g};   // 临时对象隐式移动
    assert(t1.joinable()); // 通过，t1再次拥有资源
    // t1 = std::move(t2);    // 错误，不能转移所有权到joinable的线程

    t1.join();              // t1线程执行完毕
    assert(!t1.joinable()); // 通过，t1已经不再拥有资源
    t1 = std::move(t2);     // 对象显式移动
    assert(t1.joinable());  // 通过，t1再次拥有资源
    assert(!t2.joinable()); // 通过，t2已经不再拥有资源
    t1.join();              // t1线程执行完毕
    return 0;
}