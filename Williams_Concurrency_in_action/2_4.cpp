#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

template<typename IteratorT, typename T>
struct AccumulateBlock
{
    void operator()(IteratorT first, IteratorT last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename IteratorT, typename T>
T parallelAccumulate(IteratorT first, IteratorT last, T initVal)
{
    const auto length = std::distance(first, last);
    if (length == 0)
        return initVal;

    const auto minPerThread = 25;
    const auto maxThreads = (length + minPerThread - 1) / minPerThread;
    const auto hardwareThreads = std::thread::hardware_concurrency();
    const auto threadsNumber = std::min<uint32_t>(
        hardwareThreads != 0 ? hardwareThreads : 2, maxThreads);
    const auto blockSize = length / threadsNumber;

    std::vector<T> results(threadsNumber);
    std::vector<std::thread> threads(threadsNumber - 1);

    IteratorT startBlock = first;

    for (auto i = 0ul; i < (threadsNumber - 1); ++i) {
        IteratorT endBlock = startBlock;
        std::advance(endBlock, blockSize);
        threads[i] = std::thread(
            AccumulateBlock<IteratorT, T>(),
            startBlock,
            endBlock,
            std::ref(results[i])
        );
        
        startBlock = endBlock;
    }

    AccumulateBlock<IteratorT, T>()(startBlock, last, results[threadsNumber-1]);

    for (auto& entry : threads) {
        entry.join();
    }

    return std::accumulate(results.begin(), results.end(), initVal);
}   

TEST(Williams, SimpleParallelAccumulate)
{
    std::vector testableValues(1000, 1);
    const auto result = parallelAccumulate(testableValues.begin(), testableValues.end(), 0);
    const auto target = std::accumulate(testableValues.begin(), testableValues.end(), 0);
    ASSERT_EQ(result, target);
}