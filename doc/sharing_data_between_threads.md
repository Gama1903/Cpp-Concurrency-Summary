# Sharing data between threads

## 保护并发访问共享数据
- invarients：不变量，指的是特定数据结构的固有结构特点。不变量的破坏会导致数据结构被破坏，通常发生于更新数据结构时
- race condition：竞争状态（笔者译），即线程之间发生了竞争，破坏了不变量，这样的情形称为race condition。所谓竞争，即程序的输出依赖于线程的执行顺序，但在多线程环境下，若没有合适的同步机制，则线程的执行顺序是不确定的，因此程序的输出是不确定的，这通常是不可接受的
- data race：数据竞争，是race condition的一种常见的特殊情况，即并发修改单个共享数据对象。数据竞争同样会使得不变量遭到破坏，导致程序出错
- 保护共享数据：两种方式，第一是对数据结构采用保护机制，即采用锁；第二是修改数据结构及更新算法，使得数据结构在更新时，不会破坏不变量，即采用无锁编程

## 互斥锁
- mutex：互斥锁，用于在访问共享数据前加锁，访问结束后解锁
- std::mutex：C++11提供std::mutex来创建一个mutex，可手动通过std::mutex::lock加锁，通过std::mutex::unlock解锁，但一般不推荐使用std::mutex直接调用这两个成员函数,而是使用std::lock_guard或std::unique_lock来自动管理锁([mutex](../src/sharing_data_between_threads/mutex.cpp))
- std::lock_guard：遵循RAII原则，用于自动管理锁([lock_guard](../src/sharing_data_between_threads/lock_guard.cpp))
- 数据保护失效：将被保护的数据的指针或引用传递到互斥锁作用域之外，这将导致互斥锁的保护失效([invalid_mutex](../src/sharing_data_between_threads/invalid_mutex.cpp))
- 保护接口：使用互斥锁保护接口的栈([MTStack](../src/sharing_data_between_threads/MTStack.cpp))

## 死锁
- deadlock：死锁，产生死锁的四个必要条件：互斥、占有且等待、不可抢占、循环等待
- 避免死锁的建议：一个线程尽量不要同时持有两把锁，若需要持有多把锁，每个线程的上锁顺序必须一致
- std::lock：若无法保证上锁顺序一致，则可以使用std::lock来同时上锁，这可保证不会产生死锁([lock](../src/sharing_data_between_threads/lock.cpp))
- std::scoped_lock：C++17提供std::scoped_lock，可以同时锁多个mutex，避免死锁，同时引入了模板参数类型推导([scoped_lock](../src/sharing_data_between_threads/scoped_lock.cpp))
- std::unique_lock：与std::lock_guard类似，但更加灵活。如，可手动上锁解锁，从而调整锁的粒度([unique_lock](../src/sharing_data_between_threads/unique_lock.cpp))；转移锁的所有权到另一作用域([transfer_ownership](../src/sharing_data_between_threads/transfer_ownership.cpp))；提前锁，延后锁([early_and_defer](../src/sharing_data_between_threads/early_and_defer.cpp))
- 层次锁：如果一个锁被低层持有，就不允许在高层再上锁，从而避免死锁([hierarchical_mutex](../src/sharing_data_between_threads/hierarchical_mutex.cpp))

## 读写锁
- read-write mutex：读写锁，允许多个线程同时读取共享数据，但只允许一个线程写入共享数据，从而避免数据竞争，提高了并发性能
- std:shared_mutex：C++17提供std::shared_mutex来创建一个read-write mutex，可手动通过std::shared_mutex::lock_shared加读锁，通过std::shared_mutex::unlock_shared解锁；通过std::shared_mutex::lock加写锁，通过std::shared_mutex::unlock解锁([shared_mutex](../src/sharing_data_between_threads/shared_mutex.cpp))
- std::shared_lock：遵循RAII原则，用于读线程中自动管理锁。写线程则使用std::unique_lock来自动管理锁([shared_lock](../src/sharing_data_between_threads/shared_lock.cpp))

## 递归锁
- recursive mutex：递归锁，允许在同一个线程中多次加锁，但在其他线程获取锁之前必须释放所有的锁。递归锁的存在说明代码设计本身存在问题，需要进行优化([recursive_mutex](../src/sharing_data_between_threads/recursive_mutex.cpp))

## 保护并发初始化
