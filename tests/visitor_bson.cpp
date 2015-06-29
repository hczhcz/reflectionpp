#include "common.hpp"

#include <bsoncxx/json.hpp>

namespace rpp_another_namespace {

static const int test_bson = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    rpp::MetaImpl<
        rpp::TypeList<rpp::VisitorBSON<>>,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct5)
    > meta{TestStruct5{}};

    rpp::VisitorBSON<> doc{};
    meta.doVisit(doc);

    std::cerr << bsoncxx::to_json(doc) << std::endl;

    return 0;
}();

}
