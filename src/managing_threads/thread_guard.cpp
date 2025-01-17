#include <iostream>
#include <thread>
#include <cassert>

class thread_guard
{
public:
    explicit thread_guard(std::thread &t) : t_(t) {}
    ~thread_guard()
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }
    // 显式删除拷贝构造函数和赋值操作符，避免编译器自动生成
    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;

private:
    std::thread &t_;
};

int main()
{
    std::thread t{[] {}};
    thread_guard tg(t);
}