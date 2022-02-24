#include "Async.h"

namespace async
{

void go(Handler handler)
{
    getIOService().post(std::move(handler));
}

void run()
{
    getIOService().run();
}

void dispatch(int const threadCount)
{
    const auto threads = threadCount > 0 ? threadCount : int(std::thread::hardware_concurrency());
    log("Threads: " + std::to_string(threads));
    for (auto i = 1; i < threads; ++i)
        go(run);

    run();
}

}
