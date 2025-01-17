#include <iostream>
#include <thread>

class scoped_thread
{
public:
    explicit scoped_thread(std::thread t) : t_(std::move(t))
    {
        if (!t_.joinable())
        {
            throw std::logic_error("No thread");
        }
    }

    ~scoped_thread()
    {
        t_.join();
    }

    scoped_thread(scoped_thread const &) = delete;
    scoped_thread &operator=(scoped_thread const &) = delete;

private:
    std::thread t_;
};

int main()
{
    try
    {
        scoped_thread st{std::thread{[] {}}};
    }
    // 补充代码
    catch (std::logic_error &e)
    {
        std::cout << e.what() << std::endl;
    }
}