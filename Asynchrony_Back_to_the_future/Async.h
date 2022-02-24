#pragma once
#include "Helpers.h"

namespace async
{

using Error = boost::system::error_code;
using IoHandler = std::function<void(const Error&)>;

void go(Handler handler);

void run();

void dispatch(int threadCount = 0);

}
