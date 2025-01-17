#include <iostream>
#include <thread>

void worker(int &x)
{
    // 假设这个线程在运行时，x已经被销毁
    for (size_t i = 0; i < 20; i++)
    {
        x++;
        std::cout << "x = " << x << std::endl;
    }
}

int main()
{
    int x = 42;
    std::thread t(worker, std::ref(x));
    // throw -1;
    t.detach(); // 分离线程
    // 在这里，main函数可能返回，x可能被销毁
    return 0;
}
