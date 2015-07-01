#include "common.hpp"

namespace rpp_another_namespace {

static const int test_json = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    RPP_META_LOCAL(
        "value", TestStruct5,
        rpp::TypeList<rpp::VisitorJSON<>>
    ) meta{TestStruct5{}};

    rpp::VisitorJSON<> v{std::cerr};
    meta.doVisit(v);

    std::cerr << std::endl;

    return 0;
}();

}
