#include <gtest/gtest.h>
#include <iostream>
#include <mutex>

class Foo
{
    std::once_flag onceFlag;
private:
    void doSthOnce() const
    {
        std::cout << "It must be written once" << std::endl;
    }

public:
    void actionOne()
    {
        std::call_once(onceFlag, &Foo::doSthOnce, this);
    }

    void actionTwo()
    {
        std::call_once(onceFlag, &Foo::doSthOnce, this);
    }
};

TEST(P3_3_1, L3_12)
{
    Foo foo;
    std::thread t1(&Foo::actionOne, &foo);
    std::thread t2(&Foo::actionTwo, &foo);

    t1.join();
    t2.join();
}