#include "CoroAsync.h"
#include "Async.h"

namespace coro_async
{

thread_local CoroHandler* currentThreadDeferHandler;
thread_local const async::Error* currentThreadError;

namespace
{

bool isCurrentThreadDeferHandlerExist()
{
    return currentThreadDeferHandler != nullptr;
}

}

void runCoroInThreadPool(Handler handler)
{
    log("coro_async::go ");
    async::runInThreadPool([handler = std::move(handler)]() mutable
    {
        auto* coro = new coro::Coro(std::move(handler));
        onCoroComplete(coro);
    });
}


void onCoroComplete(coro::Coro* coro)
{
    VERIFY(!coro::isInsideCoro(), "coro_async::onCoroComplete: Complete outside coro");
    VERIFY(coro->isStarted() == isCurrentThreadDeferHandlerExist(), "coro_async::onCoroComplete Unexpected condition in defer/started state");

    if (isCurrentThreadDeferHandlerExist())
    {
        log("coro_async::onCoroComplete: invoking defer handler");
        (*currentThreadDeferHandler)(coro);
        currentThreadDeferHandler = nullptr;
        log("coro_async::onCoroComplete: completed defer handler");
    }
    else
    {
        log("coro_async::onCoroComplete: nothing to do, deleting coro");
        delete coro;
    }

}


void handleError()
{
    if (currentThreadError)
        throw boost::system::system_error(*currentThreadError, "coro_async");
}

void defer(CoroHandler handler)
{
    VERIFY(coro::isInsideCoro(), "defer() outside coro");
    VERIFY(currentThreadDeferHandler == nullptr, "There is unexecuted defer handler");
    currentThreadDeferHandler = &handler;
    coro::yield();
    handleError();
}

void onComplete(coro::Coro* coro, async::Error const& error)
{
    setlocale(0, "");
    log("async completed, coro: " + std::to_string(reinterpret_cast<int>(coro)) + ", error: " + error.message());
    VERIFY(coro != nullptr, "Coro is null");
    VERIFY(!coro::isInsideCoro(), "Completion inside coro");
    currentThreadError = error ? &error : nullptr;
    coro->resume();
    log("After resume");
    onCoroComplete(coro);
}

async::AsyncHandler onCompleteHandler(coro::Coro* coro)
{
    return [coro](async::Error const& error)
    {
        onComplete(coro, error);
    };
}

}
