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
    double b{2.1};
};

struct TestStruct5: public TestStruct3, public TestStruct4 {
    TestStruct4 c;
    std::string d{"string\t字符串\n"};
    std::vector<float> e{5, 5.1, 5.2};
    std::map<std::string, unsigned> f{{"item1", 3}, {"item2", 4}};
    int g[3]{7, 8, 9};
    static constexpr bool h{true};
    std::unique_ptr<int> i[2]{nullptr, nullptr};

    TestStruct5() = default;
};

RPP_TYPE_DYNAMIC(
    std::vector<float>,
    float
)

RPP_TYPE_DYNAMIC(
    decltype(TestStruct5::f),
    unsigned
)

RPP_TYPE_DYNAMIC(
    std::unique_ptr<int>,
    int
)

RPP_TYPE_OBJECT(
    TestStruct3,
    __(a, REF)
    __(b)
)

RPP_TYPE_OBJECT(
    TestStruct4,
    __(b)
)

RPP_TYPE_OBJECT(
    TestStruct5,
    __(TestStruct3, BASE)
    __(TestStruct4, BASE_INLINE)
    __(c)
    __(d)
    __(e)
    __(f)
    __(g)
    __(h, CONST)
    __(i)
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
