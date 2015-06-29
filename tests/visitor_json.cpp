#include "common.hpp"

namespace rpp_another_namespace {

static const int test_json = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    rpp::MetaImpl<
        rpp::TypeList<rpp::VisitorJSON<>>,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct5)
    > meta{TestStruct5{}};

    rpp::VisitorJSON<> v{std::cerr};
    meta.doVisit(v);

    std::cerr << std::endl;

    return 0;
}();

}
