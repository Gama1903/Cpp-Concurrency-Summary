#include <iostream>
#include <thread>

// 函数对象
struct Foo
{
    void operator()() { std::cout << 1; }
};

int main()
{
    // 使用普通函数初始化，见construct_thread

    // 使用函数对象初始化
    Foo foo;
    // 1，2，3，4等价
    std::thread t1(foo);     // 1，圆括号初始化
    std::thread t2{foo};     // 2，花括号初始化，即列表初始化
    std::thread t3{Foo()};   // 3，这里不能使用圆括号初始化，见6
    std::thread t4((Foo())); // 4，若使用圆括号初始化，需要再加一层括号

    // 使用匿名函数初始化
    std::thread t5([]
                   { std::cout << 1; }); // 5，lambda表达式

    // 6出现解析混淆："C++ most vexing parse" 问题
    std::thread t6(Foo()); // 6，此处声明了一个名为t6的函数，而不是创建了一个线程
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    // t6.join(); // 此处会报错，因为t6是一个函数，而不是一个线程
}