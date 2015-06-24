#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include "../reflection++/visitor/strtree.hpp"
#include "../reflection++/accessor_infer.hpp"
#include "../reflection++/meta.hpp"

namespace rpp_another_namespace {

RPP_ACCESSOR_INFER_INIT()

char value_ts6_c[32] = "hello!";

// TODO: move into a header file
struct TestStruct6 {
    static int a;
    double b{200};
    char *c{value_ts6_c};

    TestStruct6() = default;
};

int TestStruct6::a{100};

struct TestStruct7: public TestStruct6 {
    TestStruct6 d;

    TestStruct7() = default;
};

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

static const int test2 = []() {
    using Root = std::unordered_map<
        std::string,
        std::map<
            std::string,
            std::string
        >
    >;

    rpp::MetaImpl<
        rpp::TypeList<
            rpp::VisitorIStrTree<Root &>,
            rpp::VisitorOStrTree<Root &>
        >,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct7)
    > meta{TestStruct7{}};

    Root root1;
    Root root2;

    rpp::VisitorOStrTree<Root &> v1{root1};
    meta.doVisit(v1);

    char tag = '0';
    for (auto &i: root1) {
        std::cout << i.first << ": " << std::endl;
        for (auto &j: i.second) {
            std::cout << "    " << j.first << ": " << j.second << std::endl;
            j.second += tag;
            ++tag;
        }
    }

    rpp::VisitorIStrTree<Root &> v2{root1};
    meta.doVisit(v2);

    rpp::VisitorOStrTree<Root &> v3{root2};
    meta.doVisit(v3);

    for (auto &i: root2) {
        std::cout << i.first << ": " << std::endl;
        for (auto &j: i.second) {
            std::cout << "    " << j.first << ": " << j.second << std::endl;
        }
    }

    return 0;
}();

}
