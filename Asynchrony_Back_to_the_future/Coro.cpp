#include "Coro.h"
#include <iostream>

namespace coro
{

thread_local Coro* currentThreadCoro;
constexpr size_t STACK_SIZE = 1024 * 32;

void yield()
{
    VERIFY(isInsideCoro(), "yield() outside coro");
    currentThreadCoro->yield0();
}

bool isInsideCoro()
{
    return currentThreadCoro != nullptr;
}

Coro::Coro()
{
    init0();
}

Coro::Coro(Handler handler)
    : Coro()
{
    start(std::move(handler));
}

Coro::~Coro()
{
    if (isStarted())
    {
        static std::mutex mutex;

        do
        {
            std::lock_guard<std::mutex> guard(mutex);
            std::cerr
                //<< boost::posix_time::microsec_clock::local_time()
                << ": "
                << "Destroying started coro"
                << std::endl;

        } while (false);
    }
}

void Coro::start(Handler handler)
{
    VERIFY(!isStarted(), "Trying to start already started coro");
    coroContext_ = context::make_fcontext(
        &stack_.back(),
        stack_.size(),
        &Coro::starterWrapper0
    );

    jump0(reinterpret_cast<void*>(&handler));
}

void Coro::resume()
{
    VERIFY(started_, "Cannot resume: not started");
    VERIFY(!running_, "Cannot resume: in running state");
    jump0();
}

bool Coro::isStarted() const
{
    return started_ || running_;
}

void Coro::starterWrapper0(context::transfer_t const transfer)
{
    currentThreadCoro->starter0(transfer);
}

void Coro::jump0(void* p)
{
    auto* thisCoro = this;
    std::swap(thisCoro, currentThreadCoro);
    running_ = true;
    coroContext_ = context::jump_fcontext(coroContext_, p).fctx;
    running_ = false;
    std::swap(thisCoro, currentThreadCoro);
    if (savedException_ != std::exception_ptr())
        std::rethrow_exception(savedException_);
}

void Coro::starter0(context::transfer_t const transfer)
{
    started_ = true;
    try
    {
        savedException_ = nullptr;
        externalContext_ = transfer.fctx;

        auto const handler(std::move(*reinterpret_cast<Handler*>(transfer.data)));
        handler();
    }
    catch (...)
    {
        savedException_ = std::current_exception();
    }

    started_ = false;
    yield0();
}

void Coro::init0()
{
    started_ = false;
    running_ = false;

    coroContext_ = nullptr;
    stack_.resize(STACK_SIZE);
}

void Coro::yield0()
{
    externalContext_ = context::jump_fcontext(externalContext_, nullptr).fctx;
}

}
