#include <iostream>
#include "../reflection++/visitor/json.hpp"
#include "../reflection++/accessor_infer.hpp"
#include "../reflection++/meta.hpp"

namespace rpp_another_namespace {

RPP_ACCESSOR_INFER_INIT()

struct TestStruct3 {
    static int a;
    double b{2};

    TestStruct3() = default;
};

int TestStruct3::a{1};

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

template <class T, size_t size>
RPP_TYPE_DYNAMIC_GENERIC(T, T (&)[size])
template <class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::vector<T, Args...> &)
template <class Key, class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::map<Key, T, Args...> &)
template <class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::unique_ptr<T, Args...> &)

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

static const int test2 = []() {
    rpp::MetaImpl<
        rpp::TypeList<rpp::VisitorJSON<>>,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct5)
    > meta{TestStruct5{}};

    rpp::VisitorJSON<> v{std::cerr};
    meta.doVisit(v);

    return 0;
}();

}
