#include <gtest/gtest.h>
#include <iostream>

using std::cout; using std::endl;
using std::string;
using std::enable_if_t;
using std::is_base_of_v;
using std::decay_t;
using std::is_integral_v;
using std::remove_reference_t;
using std::is_constructible_v;

namespace
{
    std::string getName(std::uint64_t id) { return "Foo0"; }
}


class Foo
{
public:
    template<
        typename T,
        typename = enable_if_t<!is_base_of_v<Foo, decay_t<T>> && !is_integral_v<remove_reference_t<T>>>
    >
    explicit Foo(T&& name)
        : name_(std::forward<T>(name))
    {
        cout << "Foo forward ctor, name_ = " << name_ << endl;

        static_assert(is_constructible_v<string, T>, "Is not able to use the name parameter to construct the name_");
    }

    explicit Foo(int id)
        : name_(getName(id))
    {
        cout << "Foo integral ctor, name_ = " << name_ << endl;
    }

    explicit Foo(const Foo& rhs)
        : name_(rhs.name_)
    {
        cout << "Foo copy ctor, name_ = " << name_ << endl;
    }

    explicit Foo(Foo&& rhs)
        : name_(std::move(rhs.name_))
    {
        cout << "Foo move ctor, name_ = " << name_ << endl;
    }

    string name_;
};

class Bar : public Foo
{
public:
    using Foo::Foo;

    explicit Bar(const Bar& rhs)
       : Foo(rhs)
    {
        cout << "Bar copy ctor, name_ = " << name_ << endl;
    }

    explicit Bar (Bar && rhs)
        : Foo(std::move(rhs))
    {
        cout << "Bar copy ctor, name_ = " << name_ << endl;
    }
};


TEST(Meyers_11_14, Paragrath_5_5_constanting_templates)
{
    Foo foo0(0);

    Foo foo1("Foo1");
    Foo foo2(foo1);
    Foo foo3(std::move(foo1));

    cout << "---------------" << endl;

    Bar bar0(0);

    Bar bar1("Bar1");
    Bar bar2(bar1);
    Bar bar3(std::move(bar1));

    //Foo(3.14);
}
