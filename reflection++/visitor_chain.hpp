#pragma once

#include "type_list.hpp"
#include "visitor.hpp"

namespace rpp {

// the abstract head of the visitor chain
static constexpr struct {} visitor_head{};

// the abstract tail of the visitor chain
struct VisitorTail final {
    // compile-time only
    VisitorTail() = delete;
};

// an abstract function to get the next member of the visitor chain
VisitorTail visitor_next(...);

// verify if the argument is a VisitorBase
template <class Return = void>
void visitor_verify(const VisitorBase<Return> &);

// register a visitor class into the visitor chain
#define RPP_VISITOR_REG(Type) \
    template <class Last> \
    Last visitor_trace_##Type(VisitorTail, Last); \
    \
    template <class T, class Last> \
    auto visitor_trace_##Type(T value, Last last) -> decltype( \
        visitor_verify(value), \
        visitor_trace_##Type(visitor_next(value), value) \
    ); \
    \
    Type visitor_next( \
        decltype(visitor_trace_##Type(visitor_next(visitor_head), visitor_head)) \
    );

// pass all visitor classes in the chain to a type list and create a type alias
// using Type = TypeList<Visitor1, Visitor2, ...>
#define RPP_VISITOR_COLLECT(Type) \
    template <class... Args> \
    TypeList<Args...> visitor_trace2_##Type(VisitorTail, Args...); \
    \
    template <class T, class... Args> \
    auto visitor_trace2_##Type(T value, Args... args) -> decltype( \
        visitor_verify(value), \
        visitor_trace2_##Type(visitor_next(value), args..., value) \
    ); \
    \
    using Type = \
        decltype(visitor_trace2_##Type(visitor_next(visitor_head)));

}

// ======== Usage example ========

#ifdef RPP_DEBUG

    #include <type_traits>

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
        static_assert(std::is_same<VisitorAll2, TypeList<Visitor1, Visitor2, Visitor3>>(), "");

    }

#endif
