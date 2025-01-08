#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

/**
 * 互斥量：std::mutex
 * 1. 锁定和解锁：std::mutex::lock(), std::mutex::unlock(), std::mutex::try_lock(), std::timed_mutex::try_lock_for(), std::timed_mutex::try_lock_until()
 * 2. 自动管理锁：std::lock_guard(互斥量，锁标签)，构造时自动加锁，析构时自动解锁
 * 3. 独占锁：std::unique_lock(互斥量，锁标签)，在lock_guard基础上，可以手动锁定和解锁
 * 4. 锁标签：延迟锁定（std::defer_lock），时间锁定（std::try_to_lock），接受锁定（std::adopt_lock）
 *
 */

#include <cstdio>
#include <thread>
#include <mutex>

int main()
{
    std::mutex mtx;
    std::thread t1([&]
                   {
        std::unique_lock grd(mtx, std::try_to_lock);
        if (grd.owns_lock())
            printf("t1 success\n");
        else
            printf("t1 failed\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });

    std::thread t2([&]
                   {
        std::unique_lock grd(mtx, std::try_to_lock);
        if (grd.owns_lock())
            printf("t2 success\n");
        else
            printf("t2 failed\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });

    t1.join();
    t2.join();
    return 0;
}
