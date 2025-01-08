#include <iostream>
// #include <chrono> // 包含时间库
#include <thread> // 包含线程库，其中包含了chrono库

/**
 * 时间：std::chrono，特点：强类型指定
 * 1. 时间点类型：std::chrono::steady_clock::time_point
 * 2. 获取当前时间点：std::chrono::steady_clock::now()
 * 3. 时间段类型：std::chrono::duration, std::chrono::milliseconds, std::chrono::seconds
 * 4. 获取时间段数值：dt.count()
 * 5. 时间段转换：std::chrono::duration_cast<类型，单位>(时间段)
 * 6. 睡眠：std::this_thread::sleep_for(时间段), std::this_thread::sleep_until(时间点)
 *
 */

// 64位整型毫秒
using int_ms = std::chrono::duration<int64_t, std::milli>;
// using int_ms = std::chrono::duration<std::chrono::milliseconds>; // 等效

// 双精度毫秒
using double_ms = std::chrono::duration<double, std::milli>;

int main()
{
    // 预热
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 睡眠1秒

    // 版本一，使用双精度毫秒
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now(); // 获取当前时间点
    for (volatile size_t i = 0; i < 1000000000; i++)                             // 无操作循环
        ;
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now(); // 获取当前时间点
    std::chrono::duration<int64_t, std::nano> dt1 = t1 - t0;
    double dt1_ms = std::chrono::duration_cast<double_ms>(dt1).count(); // 获取时间差的双精度毫秒数值
    std::cout << "Elapsed time: " << dt1_ms << " ms" << std::endl;      // 输出时间差

    // 版本二，使用64位整型毫秒，同时使用类型推断
    auto t2 = std::chrono::steady_clock::now();
    for (volatile size_t i = 0; i < 1000000000; i++)
        ;
    auto t3 = std::chrono::steady_clock::now();
    auto dt2 = t3 - t2;
    auto dt2_ms = std::chrono::duration_cast<int_ms>(dt2).count(); // 获取时间差的64位整型毫秒数值
    // double dt2_ms = std::chrono::duration_cast<double_ms>(dt2).count();
    std::cout << "Elapsed time: " << dt2_ms << " ms" << std::endl;
    return 0;
}