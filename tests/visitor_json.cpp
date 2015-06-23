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
    TestStruct4 c;
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

RPP_TYPE_OBJECT(
    TestStruct3,
    __(a, REF)
    __(b)
)

RPP_TYPE_OBJECT(
    TestStruct4,
    __(b)
)

RPP_TYPE_DYNAMIC(decltype(TestStruct5::e), float)
RPP_TYPE_DYNAMIC(decltype(TestStruct5::f), unsigned)
RPP_TYPE_DYNAMIC(decltype(TestStruct5::g), int)
RPP_TYPE_DYNAMIC(std::unique_ptr<bool>, bool)
RPP_TYPE_DYNAMIC(decltype(TestStruct5::j), std::unique_ptr<bool>)

RPP_TYPE_OBJECT(
    TestStruct5,
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
    __(k)
)

static const int test2 = []() {
    rpp::MetaImpl<
        rpp::TypeList<rpp::VisitorJSON<>>,
        RPP_ACCESSOR_GET(
            RPP_HOLDER_STR("value"),
            RPP_HOLDER_LOCAL(TestStruct5)
        )
    > meta{TestStruct5{}};

    rpp::VisitorJSON<> v{std::cerr};

    meta.doVisit(v);

    return 0;
}();

}
