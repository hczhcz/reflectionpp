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

// enable the visitor chain in the current namespace
#define RPP_VISITOR_CHAIN_INIT() \
    namespace visitor_chain { \
        static constexpr struct Here final {} here{}; \
        \
        /* an abstract function to get the next member of the visitor chain */ \
        rpp::VisitorTail visitor_next(...); \
        \
        /* call visitor_next with VisitorTail is not allowed */ \
        void visitor_next(Here, rpp::VisitorTail) = delete; \
    }

RPP_VISITOR_CHAIN_INIT()

// register a visitor class into the visitor chain
#define RPP_VISITOR_REG(Type) \
    namespace visitor_chain { \
        template <class Last> \
        Last visitor_trace_##Type(Here, rpp::VisitorTail, Last); \
        \
        template <class T, class Last> \
        auto visitor_trace_##Type(Here, T value, Last last) -> decltype( \
            visitor_trace_##Type(here, visitor_next(here, value), value) \
        ); \
        \
        Type visitor_next( \
            Here, decltype(visitor_trace_##Type( \
                here, visitor_next(here, rpp::visitor_head), rpp::visitor_head) \
            ) \
        ); \
    }

// pass all visitor classes in the chain to a type list and create a type alias
// using Type = TypeList<Visitor1, Visitor2, ...>
#define RPP_VISITOR_COLLECT(Type) \
    namespace visitor_chain { \
        template <class... Args> \
        rpp::TypeList<Args...> visitor_trace2_##Type(Here, rpp::VisitorTail, Args...); \
        \
        template <class T, class... Args> \
        auto visitor_trace2_##Type(Here, T value, Args... args) -> decltype( \
            visitor_trace2_##Type(here, visitor_next(here, value), args..., value) \
        ); \
        \
        using Type = \
            decltype(visitor_trace2_##Type( \
                here, visitor_next(here, rpp::visitor_head)) \
            ); \
    } \
    \
    using Type = visitor_chain::Type;

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

#endif
