#include <iostream>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#elif defined __GNUC__
#include <syscall.h>
#include <unistd.h>

#endif

void print_current_thread_id()
{
#ifdef _WIN32
    std::cout << std::this_thread::get_id() << std::endl;      // 19840
    std::cout << GetCurrentThreadId() << std::endl;            // 19840
    std::cout << GetThreadId(GetCurrentThread()) << std::endl; // 19840
#elif defined __GNUC__
    std::cout << std::this_thread::get_id() << std::endl; // 1
    std::cout << pthread_self() << std::endl;             // 140250646003520
    std::cout << getpid() << std::endl;                   // 1502109，ps aux 显示此 pid
    std::cout << syscall(SYS_gettid) << std::endl;        // 1502109
#endif
}

std::thread::id master_thread_id = std::this_thread::get_id();

void f()
{
    if (std::this_thread::get_id() == master_thread_id)
    {
        // do_master_thread_work();
    }
    // do_common_work();
}

int main()
{
    print_current_thread_id();
    f();
    std::thread t{f};
    t.join();
}