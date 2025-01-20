#include <iostream>
#include <thread>
#include <mutex>

int main()
{
    uint64_t sum = 0;
    std::mutex mtx;
    std::thread t1{[&]
                   {
                       for (size_t i = 0; i < 1000; i++)
                       {
                           mtx.lock();
                           sum++;
                           mtx.unlock();
                       }
                   }};
    std::thread t2{[&]
                   {
                       for (size_t i = 0; i < 1000; i++)
                       {
                           mtx.lock();
                           sum += 2;
                           mtx.unlock();
                       }
                   }};
    t1.join();
    t2.join();
    std::cout << sum << std::endl;
    return 0;
}
