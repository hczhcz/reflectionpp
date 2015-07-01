#include "common.hpp"

namespace rpp_another_namespace {

static const int test_strtree = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    // out

    using Root = std::unordered_map<
        std::string,
        std::map<
            std::string,
            std::string
        >
    >;

    RPP_META_LOCAL(
        "value", TestStruct7,
        rpp::TypeList<
            rpp::VisitorIStrTree<Root &>,
            rpp::VisitorOStrTree<Root &>
        >
    ) meta{TestStruct7{}};

    Root root1;
    Root root2;

    rpp::VisitorOStrTree<Root &> v1{root1};
    meta.doVisit(v1);

    char tag = '0';
    for (auto &i: root1) {
        std::cerr << i.first << ": " << std::endl;
        for (auto &j: i.second) {
            std::cerr << "    " << j.first << ": " << j.second << std::endl;
            j.second += tag;
            ++tag;
        }
    }

    // in

    rpp::VisitorIStrTree<Root &> v2{root1};
    meta.doVisit(v2);

    rpp::VisitorOStrTree<Root &> v3{root2};
    meta.doVisit(v3);

    for (auto &i: root2) {
        std::cerr << i.first << ": " << std::endl;
        for (auto &j: i.second) {
            std::cerr << "    " << j.first << ": " << j.second << std::endl;
        }
    }

    return 0;
}();

}
