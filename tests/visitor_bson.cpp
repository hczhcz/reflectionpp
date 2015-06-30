#include "common.hpp"

#include <bsoncxx/json.hpp>

namespace rpp_another_namespace {

static const int test_bson = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    // out

    rpp::MetaImpl<
        rpp::TypeList<
            rpp::VisitorBSON<>
        >,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct5)
    > meta1{TestStruct5{}};

    rpp::VisitorBSON<> doc{};
    meta1.doVisit(doc);

    std::cerr << bsoncxx::to_json(doc) << std::endl;

    // in

    rpp::MetaImpl<
        rpp::TypeList<
            rpp::VisitorBSON<>,
            rpp::VisitorBSONView<>
        >,
        RPP_ACCESSOR_GET_AS("value", LOCAL, TestStruct5m)
    > meta2{TestStruct5m{}};

    rpp::VisitorBSONView<> view{bsoncxx::types::b_document{doc}};
    meta2.doVisit(view);

    rpp::VisitorBSON<> doc2{};
    meta2.doVisit(doc2);

    std::cerr << bsoncxx::to_json(doc2) << std::endl;

    return 0;
}();

}
