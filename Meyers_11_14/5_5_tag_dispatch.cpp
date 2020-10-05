#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <type_traits>

using std::string;
using std::vector;
using std::is_integral;
using std::remove_reference_t;

vector<string> names;

namespace
{
std::string getName(std::uint64_t id) { return "Foo0"; }
}

template<typename T>
void fooImpl(T&& name, std::false_type)
{
    names.push_back(std::forward<T>(name));
}

void fooImpl(const std::uint64_t id, std::true_type)
{
    names.push_back(getName(id));
}

template<typename T>
void foo(T&& value)
{
    fooImpl(std::forward<T>(value), is_integral<remove_reference_t<T>>());
}

TEST(Meyers_11_14, Paragrath_5_5_tag_dispatch_0)
{
    foo(0);
    ASSERT_FALSE(names.empty());
    EXPECT_EQ(names.front(), "Foo0");
}

TEST(Meyers_11_14, Paragrath_5_5_tag_dispatch_1)
{
    string name1("Foo1");
    foo(std::move(name1));
    ASSERT_EQ(names.size(), 2);
    EXPECT_EQ(names.back(), "Foo1");
    EXPECT_TRUE(name1.empty());
}

TEST(Meyers_11_14, Paragrath_5_5_tag_dispatch_2)
{
    string name2("Foo2");
    foo(name2);
    ASSERT_EQ(names.size(), 3);
    EXPECT_EQ(names.back(), "Foo2");
    EXPECT_FALSE(name2.empty());
}
