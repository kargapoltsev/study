#include "Async.h"

namespace async
{

void runInThreadPool(Handler handler)
{
    getIOService().post(std::move(handler));
}

void runThreadInPool()
{
    getIOService().run();
}

void dispatch(int const threadCount)
{
    const auto threads = threadCount > 0 ? threadCount : int(std::thread::hardware_concurrency());
    log("Threads: " + std::to_string(threads));
    for (auto i = 1; i < threads; ++i)
        runInThreadPool(runThreadInPool);

    runThreadInPool();
}

}
