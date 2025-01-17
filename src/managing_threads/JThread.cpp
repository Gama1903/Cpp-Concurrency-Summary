#include <iostream>
#include <thread>

class JThread
{
public:
    using id = std::thread::id;

    // 无参数构造函数
    JThread() noexcept = default;

    // 构造函数
    template <typename Callable, typename... Args>
    explicit JThread(Callable &&func, Args &&...args) : t_{std::forward<Callable>(func), std::forward<Args>(args)...} {}
    explicit JThread(std::thread t) noexcept : t_{std::move(t)} {}

    // 析构函数
    ~JThread() noexcept
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }

    // 显式删除拷贝构造函数和拷贝赋值运算符
    JThread(JThread const &) = delete;
    JThread &operator=(JThread const &) = delete;

    // 移动构造函数和移动赋值运算符
    JThread(JThread &&other) noexcept : t_{std::move(other.t_)} {}
    JThread &operator=(JThread &&other) noexcept
    {
        if (t_.joinable())
        {
            t_.join();
        }
        t_ = std::move(other.t_);
        return *this;
    }
    JThread &operator=(std::thread t) noexcept
    {
        if (t_.joinable())
        {
            t_.join();
        }
        t_ = std::move(t);
        return *this;
    }

    void swap(JThread &other) noexcept
    {
        t_.swap(other.t_);
    }

    bool joinable() const noexcept
    {
        return t_.joinable();
    }

    void join()
    {
        t_.join();
    }

    void detach()
    {
        t_.detach();
    }

    id get_id() const noexcept
    {
        return t_.get_id();
    }

    std::thread &as_thread() noexcept
    {
        return t_;
    }

    const std::thread &as_thread() const noexcept
    {
        return t_;
    }

private:
    std::thread t_;
};

int main()
{
    JThread t{[] {}};
}