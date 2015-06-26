#include <type_traits>

#include "../reflection++/visitor_chain.hpp"

namespace rpp {

struct Visitor1: public VisitorIgnoreBase<> {};
struct Visitor2: public VisitorIgnoreBase<> {};
namespace some_namespace {
    struct Visitor3: public VisitorIgnoreBase<> {};
}
using Visitor3 = some_namespace::Visitor3;

RPP_VISITOR_COLLECT(VisitorAll1)

RPP_VISITOR_REG(Visitor1)
RPP_VISITOR_REG(Visitor2)
RPP_VISITOR_REG(Visitor3)

RPP_VISITOR_COLLECT(VisitorAll2)

static_assert(std::is_same<VisitorAll1, TypeList<>>(), "");
static_assert(std::is_same<VisitorAll2, TypeList<
    Visitor1, Visitor2, some_namespace::Visitor3
>>(), "");

}

namespace rpp_another_namespace {

RPP_VISITOR_CHAIN_INIT()

using Visitor1 = rpp::Visitor1;
struct Visitor2: public rpp::VisitorIgnoreBase<> {};

RPP_VISITOR_REG(Visitor1)
RPP_VISITOR_REG(Visitor2)

RPP_VISITOR_COLLECT(VisitorAll1)

static_assert(std::is_same<VisitorAll1, rpp::TypeList<
    rpp::Visitor1, Visitor2
>>(), "");

}
