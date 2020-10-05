#include <gtest/gtest.h>
#include <string>

using std::string;

namespace 
{
std::string getName(std::uint64_t id) { return "Foo"; }
}

class Foo
{
public:
    explicit Foo(string name)
        : name_(std::move(name))
    {}

    explicit Foo(const size_t id)
        : name_(getName(id))
    {}

    bool isName(const std::string& name) const
    {
        return name_ == name;
    }

private:
    std::string name_;
};

TEST(Meyers_11_14, Paragrath_5_5_pass_by_value_0)
{
    const string name("Foo");
    Foo foo(name);
    EXPECT_FALSE(name.empty());
}

TEST(Meyers_11_14, Paragrath_5_5_pass_by_value_1)
{
    string name("Foo");
    Foo foo(std::move(name));
    EXPECT_TRUE(name.empty());
}

TEST(Meyers_11_14, Paragrath_5_5_pass_by_value_2)
{
    const Foo foo(0);
    EXPECT_TRUE(foo.isName("Foo"));
}
