#pragma once
#include "Helpers.h"
#include <boost/context/detail/fcontext.hpp>

namespace coro
{

void yield();

bool isInsideInCoro();

namespace context = boost::context::detail;

struct Coro
{
    friend void yield();

    Coro();
    explicit Coro(Handler handler);
    ~Coro();

    void start(Handler handler);
    void resume();
    [[nodiscard]] bool isStarted() const;

private:
    static void starterWrapper0(context::transfer_t transfer);
    void jump0(void* p = nullptr);
    void starter0(context::transfer_t transfer);
    void init0();
    void yield0();

private:
    context::fcontext_t coroContext_ = nullptr;
    context::fcontext_t externalContext_ = nullptr;
    std::vector<uint8_t> stack_;
    std::exception_ptr savedException_ = nullptr;
    
    bool running_ = false;
    bool started_ = false;
};

}
