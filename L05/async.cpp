#include <iostream>
#include <thread>
#include <string>
#include <future> // 用于异步编程

/**
 * 异步：std::future
 * 1. 创建异步任务：std::future, std::async(启动策略，任务函数)
 * 2. 异步任务的同步：std::future::get(), std::future::wait(), std::future::wait_for(), std::future::wait_until()
 * 3. 伪异步：std::async(std::launch::deferred, func)
 * 4. 手动异步：std::promise, std::promise::set_value(), std::promise::get_future()
 * 5. 共享异步：std::shared_future, std::shared_promise
 *
 */

int download(std::string file)
{
    for (int i = 0; i < 4; i++)
    {
        std::cout << "Downloading " << file
                  << " (" << i * 25 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete: " << file << std::endl;
    return 404;
}

void interact()
{
    // std::string name;
    // std::cin >> name;
    // std::cout << "Hi, " << name << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Interact complete" << std::endl;
}

int main()
{
    // 并行线程任务
    // 自动异步
    auto furRes1 = std::async([&]
                              { return download("hello.zip"); });
    // 伪异步
    auto furRes2 = std::async(std::launch::deferred, [&]
                              { return download("hello.zip"); });
    // 手动异步
    std::promise<int> prom;
    std::thread t1{[&]
                   {
                       auto res = download("hello.zip");
                       prom.set_value(res);
                   }};
    auto furRes3 = prom.get_future();
    // 共享异步
    std::shared_future<int> furRes4 = std::async([&]
                                                 { return download("hello.zip"); });
    auto furRes5 = furRes4;
    auto furRes6 = furRes4;

    // 主线程任务
    interact();

    // 异步任务同步
    auto res1 = furRes1.get();
    auto res2 = furRes2.get(); // 伪异步，延迟执行
    auto res3 = furRes3.get();
    t1.join();
    auto res4 = furRes4.get();
    auto res5 = furRes5.get();
    auto res6 = furRes6.get();

    // 输出结果
    std::cout << "Download result: " << res1 << std::endl
              << "Download result: " << res2 << std::endl
              << "Download result: " << res3 << std::endl
              << "Download result: " << res4 << std::endl
              << "Download result: " << res5 << std::endl
              << "Download result: " << res6 << std::endl
              << "Download completed" << std::endl;
    return 0;
}