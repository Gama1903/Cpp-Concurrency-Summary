#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &res)
    {
        res = std::accumulate(first, last, res);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    uint64_t len = std::distance(first, last);
    if (!len)
    {
        return init;
    }
    uint64_t min_per_thread = 25;
    uint64_t max_threads = (len + min_per_thread - 1) / min_per_thread;
    uint64_t hardware_threads = std::thread::hardware_concurrency();
    uint64_t num_threads = // 线程数量
        std::min(hardware_threads == 0 ? 2 : hardware_threads, max_threads);
    uint64_t offset = len / num_threads; // 数据块间的偏移量
    std::vector<T> res(num_threads);
    std::vector<std::thread> threads(num_threads - 1); // 已有主线程故少一个线程
    Iterator block_start = first;
    for (size_t i = 0; i < num_threads - 1; ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, offset); // block_end 指向当前块尾部
        threads[i] = std::thread{accumulate_block<Iterator, T>{}, block_start,
                                 block_end, std::ref(res[i])};
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, res[num_threads - 1]);
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));
    return std::accumulate(res.begin(), res.end(), init);
}

int main()
{
    std::vector<uint64_t> v(1000000);
    std::iota(std::begin(v), std::end(v), 0);
    uint64_t res = std::accumulate(std::begin(v), std::end(v), 0);
    assert(parallel_accumulate(std::begin(v), std::end(v), 0) == res);
}