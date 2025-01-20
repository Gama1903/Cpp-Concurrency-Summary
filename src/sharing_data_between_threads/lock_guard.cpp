#include <iostream>
#include <thread>
#include <mutex>

// std::mutex的鸭子类型
struct duck_mtx
{
    void lock()
    {
        std::cout << "lock" << std::endl;
    }
    void unlock()
    {
        std::cout << "unlock" << std::endl;
    }
};

int main()
{
    std::mutex mtx1;
    duck_mtx mtx2;

    uint64_t sum = 0;
    std::thread t1{[&]
                   {
                       for (size_t i = 0; i < 1000; i++)
                       {
                           std::lock_guard<std::mutex> grd(mtx1);
                           sum++;
                       }
                   }};
    std::thread t2{[&]
                   {
                       std::lock_guard<duck_mtx> grd(mtx2); // lock_guard的模板参数可以是任何具有lock和unlock方法的类型
                   }};
    t1.join();
    t2.join();
    std::cout << sum << std::endl;
    return 0;
}
