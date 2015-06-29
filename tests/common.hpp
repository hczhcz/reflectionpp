#pragma once

#include <type_traits>
#include <iostream>

#include "../reflection++/visitor_chain.hpp"
#include "../reflection++/visitor/basic.hpp"
#include "../reflection++/visitor/strtree.hpp"
#include "../reflection++/visitor/json.hpp"
#include "../reflection++/visitor/bson.hpp"
#include "../reflection++/visitor/mysql.hpp"
#include "../reflection++/accessor_infer.hpp"
#include "../reflection++/meta.hpp"

namespace rpp_another_namespace {

RPP_ACCESSOR_INFER_INIT()

struct TestStruct {
    int member1;
    float member2;
};

struct TestStruct2: public TestStruct {
    char member3;
    static int member4;
    TestStruct member5;

    TestStruct2(int a, char b, int c):
        TestStruct{a, a + 1.0f}, member3{b}, member5{c, c + 1.0f} {}
};

struct TestStruct3 {
    static int a;
    double b{2};

    TestStruct3() = default;
};

struct TestStruct4 {
    const double b{2.1};
};

struct TestStruct5: public TestStruct3, public TestStruct4 {
    TestStruct4 c[2];
    std::string d{"string\t字符串\n"};
    std::vector<float> e{5, 5.1, 5.2};
    std::map<std::string, unsigned> f{{"item1", 6}, {"item2", 7}};
    int g[3]{8, 9, 10};
    bool h{false};
    static constexpr bool i{true};
    std::unique_ptr<bool> j[2]{nullptr, nullptr};
    char k[6]{"test\0"};

    TestStruct5() = default;
};

static char value_ts6_c[32]{"hello!"};

struct TestStruct6 {
    static int a;
    double b{200};
    char *c{value_ts6_c};

    TestStruct6() = default;
};

struct TestStruct7: public TestStruct6 {
    TestStruct6 d;

    TestStruct7() = default;
};

template <class T, size_t size>
RPP_TYPE_DYNAMIC_GENERIC(T, T (&)[size])
template <class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::vector<T, Args...> &)
template <class Key, class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::map<Key, T, Args...> &)
template <class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::unique_ptr<T, Args...> &)

RPP_TYPE_OBJECT(
    __(member1)
    __(member2, MEMBER), // the same as __(member2)
    TestStruct
)

RPP_TYPE_OBJECT(
    __(TestStruct, BASE)
    __(member3)
    __(member4, REF)
    __(member5),
    TestStruct2
)

RPP_TYPE_OBJECT(
    __(a, REF)
    __(b),
    TestStruct3
)

RPP_TYPE_OBJECT(
    __(b),
    TestStruct4
)

RPP_TYPE_OBJECT(
    __(TestStruct3, BASE)
    __(TestStruct4, BASE_INLINE)
    __(c)
    __(d)
    __(e)
    __(f)
    __(g)
    __(h)
    __(i, CONST)
    __(j)
    __(k),
    TestStruct5
)

RPP_TYPE_OBJECT(
    __(a, REF)
    __(b)
    __(c),
    TestStruct6
)

RPP_TYPE_OBJECT(
    __(TestStruct6, BASE)
    __(d),
    TestStruct7
)

}
