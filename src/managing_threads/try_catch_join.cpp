#include <iostream>
#include <thread>
#include <exception>

int main()
{
    std::thread t([] {});
    try
    {
        // 异常
        throw -1;
    }
    catch (int err)
    {
        t.join(); // 处理异常前先正常结束线程
        std::cout << "Thread destroyed" << std::endl;
        throw err;
    }
    t.join(); // 若主线程无异常，正常结束线程
    std::cout << "Everything is fine" << std::endl;
    return 0;
}