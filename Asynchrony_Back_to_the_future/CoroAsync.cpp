#include "CoroAsync.h"
#include "Async.h"

namespace coro_async
{

void go(Handler handler)
{
    log("coro_async::go ");
    async::go([handler = std::move(handler)]() mutable
    {
        auto* coro = new coro::Coro(std::move(handler));
        onCoroComplete(coro);
    });
}


thread_local CoroHandler* currentThreadDeferHandler;

void onCoroComplete(coro::Coro* coro)
{
    VERIFY(!coro::isInsideInCoro(), "coro_async::onCoroComplete: Complete inside coro");
    VERIFY(coro->isStarted() == (currentThreadDeferHandler != nullptr), "coro_async::onCoroComplete Unexpected condition in defer/started state");
    if (currentThreadDeferHandler != nullptr)
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

thread_local const async::Error* currentThreadError;

void handleError()
{
    if (currentThreadError)
        throw boost::system::system_error(*currentThreadError, "coro_async");
}

void defer(CoroHandler handler)
{
    VERIFY(coro::isInsideInCoro(), "defer() outside coro");
    VERIFY(currentThreadDeferHandler == nullptr, "There is unexecuted defer handler");
    currentThreadDeferHandler = &handler;
    coro::yield();
    handleError();
}

void onComplete(coro::Coro* coro, async::Error const& error)
{
    log("async completed, coro: " + std::to_string(reinterpret_cast<int>(coro)) + ", error: " + error.message());
    VERIFY(coro != nullptr, "Coro is null");
    VERIFY(!coro::isInsideInCoro(), "Completion inside coro");
    currentThreadError = error ? &error : nullptr;
    coro->resume();
    log("After resume");
    onCoroComplete(coro);
}

async::IoHandler onCompleteHandler(coro::Coro* coro)
{
    return [coro](async::Error const& error)
    {
        onComplete(coro, error);
    };
}

}
