#include <gtest/gtest.h>
#include <iostream>
#include "Coro.h"

void doCoro()
{
    std::cout << '2';
    yield();
    std::cout << '4';
}

TEST(BACK_TO_FUTURE, CoroTest)
{
    std::cout << '1';
    Coro c(doCoro);
    std::cout << '3';
    c.resume();
    std::cout << '5';
}


namespace ctx = boost::context::detail;

int value1 = 0;

void f3(ctx::transfer_t t_) {
    ++value1;
    ctx::transfer_t t = ctx::jump_fcontext(t_.fctx, 0);
    ++value1;
    ctx::jump_fcontext(t.fctx, t.data);
}

void test_jump() {
    value1 = 0;

    std::vector<uint8_t> stack_;
    stack_.resize(1024 * 32);

    ctx::fcontext_t ctx = ctx::make_fcontext(&stack_.back(), stack_.size(), f3);
    ASSERT_TRUE(ctx);
    ASSERT_EQ(0, value1);
    ctx::transfer_t t = ctx::jump_fcontext(ctx, 0);
    ASSERT_EQ(1, value1);
    ctx::jump_fcontext(t.fctx, 0);
    ASSERT_EQ(2, value1);
}

TEST(BACK_TO_FUTURE, ContextTest)
{
    test_jump();
}