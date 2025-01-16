# Managing threads

## 线程的启动
- 主线程：一个程序至少有一个线程，即主线程，自动启动，执行main函数
- 创建std::thread：启动子线程，就是创建一个std::thread对象，初始化需要一个入口函数作为参数([construct_thread](../src/thread/construct_thread.cpp))。初始化的形式有多种，一般采用列表初始化（统一初始化）([initialize_thread](../src/thread/initialize_thread.cpp))
- 入口函数：可以是函数，也可以是lambda表达式（匿名函数），还可以是函数对象（重载了operator()的类对象）
- 传递参数：为入口函数传递参数
  - 普通函数
    - 默认参数：会被忽略，需要显式传递
    - 引用参数：也会被忽略，需要使用std::ref显式传递
    - 仅可移动参数：使用std::move传递
  - 成员函数：需要使用成员函数指针，并且需要传递一个指向对象实例的引用
  - 匿名函数：参数直接通过lambda表达式的参数捕获机制传递

## 线程的结束
- 销毁std::thread：结束子线程，就是销毁一个std::thread对象。正常的销毁之前，需要指定线程的结束方式，分为两种：加入式（join）和分离式（detach），否则std::thread的析构函数会调用std::terminate()终止程序
- std:\:thread\::join：阻塞主线程，等待子线程结束，然后主线程再继续执行（即子线程并入了主线程）。注意主线程发生异常退出时，为了保证子线程可以正常join，有两种方法：利用异常处理try/catch机制([try_catch_join](../src/thread/try_catch_join.cpp))；利用RAII机制([thread_guard](../src/thread/thread_guard.cpp))，构造函数以std::thread引用为参数
- std:\:thread\::detach：分离主线程和子线程，子线程独立运行，主线程继续执行，子线程结束后自动释放资源（即子线程脱离了主线程）。注意分离线程可能出现空悬引用的隐患，故尽量避免使用访问局部变量的入口函数([dangling_reference](../src/thread/dangling_reference.cpp))
- std:\:thread\::joinable：判断线程是否可以join，包括不可重复join，detach后不可join

## 线程的转移
- std::move：使得作用对象可转移，转移后原对象不再拥有资源。std::thread不可拷贝，仅可移动（转移）。作为临时变量时会隐式地转移，作为函数参数或返回值传递时需要显式调用std::move进行转移([transfer_thread](../src/thread/transfer_thread.cpp))
- scoped thread：利用RAII机制和std::move的移动机制，使得线程对象在作用域结束时自动join，且构造函数可以接受一个std::thread对象作为参数，而不是std::thread的一个引用([scoped_thread](../src/thread/scoped_thread.cpp))
- 类似于C++20中std::jthread的[JThread](../src/thread/JThread.cpp)

## 线程的数量

## 线程的标识

## 线程的绑定