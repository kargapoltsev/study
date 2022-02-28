#pragma once
#include "Helpers.h"

namespace async
{

using Error = boost::system::error_code;
using AsyncHandler = std::function<void(const Error&)>;

void runInThreadPool(Handler handler);

void runThreadInPool();

void dispatch(int threadCount = 0);

}
