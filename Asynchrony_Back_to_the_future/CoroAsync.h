#pragma once
#include "Coro.h"
#include "Helpers.h"
#include "Async.h"

namespace coro_async
{

using CoroHandler = std::function<void(coro::Coro*)>;

void runCoroInThreadPool(Handler handler);

void onCoroComplete(coro::Coro* coro);

void handleError();

void defer(CoroHandler handler);

void onComplete(coro::Coro* coro, async::Error const& error);

async::AsyncHandler onCompleteHandler(coro::Coro* coro);

}
