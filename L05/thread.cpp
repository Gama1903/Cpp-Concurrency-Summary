#include <iostream>
#include <thread> // 包含线程库
#include <string>
#include <vector>

/**
 * 线程：std::thread
 * 1. 创建线程：std::thread t(func);
 * 2. 线程的生命周期：join()、detach()
 * 3. 线程池
 *
 */

// 模拟下载
void download(std::string const &file)
{
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << "Downloading " << file << "(" << i * 10 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete: " << file << std::endl;
}

// 模拟交互
void interact()
{
    std::string name;
    std::cin >> name;
    std::cout << "Hi, " << name << std::endl;
}

// 线程池
class ThreadPool
{
    std::vector<std::thread> pool_m;

    // 根据三五法则，显式定义五大函数
public:
    ThreadPool() = default;
    ~ThreadPool()
    {
        for (auto &t : pool_m)
        {
            t.join();
        }
    }
    ThreadPool(ThreadPool const &) = delete;
    ThreadPool &operator=(ThreadPool const &) = delete;
    ThreadPool(ThreadPool &&) = default;
    ThreadPool &operator=(ThreadPool &&) = default;

    void push_back(std::thread thr)
    {
        pool_m.push_back(std::move(thr));
    }
};

ThreadPool tpool{}; // 全局线程池

// 异步下载
void myfunc()
{
    std::thread t1{[&]
                   { download("hello.zip"); }};
    tpool.push_back(std::move(t1)); // 线程不允许被复制，只能移动
}

int main()
{
    myfunc();
    interact();
    return 0;
}