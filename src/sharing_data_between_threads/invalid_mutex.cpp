#include <iostream>
#include <mutex>
#include <tuple>

// 某类数据
class some_data
{
private:
    int a_;
    float b_;

public:
    some_data() : a_(0), b_(0.0f) {}
    std::tuple<int, float> get() const
    {
        return {a_, b_};
    }
    void set(int a, float b)
    {
        this->a_ = a;
        this->b_ = b;
    }
};

// 数据的线程安全封装
class data_wrapper
{
private:
    some_data data_;
    mutable std::mutex m_;

public:
    data_wrapper() : data_() {}
    std::tuple<int, float> get_data() const
    {
        std::lock_guard<std::mutex> lock(m_);
        return data_.get();
    }
    void set_data(int a, float b)
    {
        std::lock_guard<std::mutex> lock(m_);
        data_.set(a, b);
    }
    template <typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> lock(m_);
        func(data_); // 传递了受保护数据的引用
    }
};

some_data *unprotected_data{};
void malicious_func(some_data &protected_data)
{
    unprotected_data = &protected_data;
}

int main()
{
    data_wrapper dw{};
    dw.process_data(malicious_func); // 在这里传递了一个恶意函数，该函数会破坏数据封装
    unprotected_data->set(1, 2.2f);  // 在无保护的情况下修改数据
}