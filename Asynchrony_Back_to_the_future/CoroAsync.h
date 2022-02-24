#pragma once
#include "Coro.h"
#include "Helpers.h"
#include "Async.h"

namespace coro_async
{

using CoroHandler = std::function<void(coro::Coro*)>;

void go(Handler handler);

void onCoroComplete(coro::Coro* coro);

void handleError();

void defer(CoroHandler handler);

void onComplete(coro::Coro* coro, async::Error const& error);

async::IoHandler onCompleteHandler(coro::Coro* coro);

}
