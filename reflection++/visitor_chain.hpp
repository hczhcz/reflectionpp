#pragma once

#include "type_list.hpp"
#include "visitor.hpp"

namespace rpp {

// the abstract tail of the visitor chain
struct VisitorTail final {
    // compile-time only
    VisitorTail() = delete;
};

// enable the visitor chain in the current namespace
#define RPP_VISITOR_CHAIN_INIT() \
    namespace visitor_chain { \
        /* an empty object to control argument-dependent lookup */ \
        /* and provide a head of the visitor chain */ \
        static constexpr struct Here final { \
            Here() = delete; \
        } here{}; \
        \
        /* an abstract function to get the next member of the visitor chain */ \
        rpp::VisitorTail next(...); \
        \
        /* call next with VisitorTail is not allowed */ \
        void next(Here, rpp::VisitorTail) = delete; \
    }

RPP_VISITOR_CHAIN_INIT()

// register a visitor class into the visitor chain
#define RPP_VISITOR_REG(Type) \
    namespace visitor_chain { \
        template <class Last> \
        Last trace_##Type(Here, rpp::VisitorTail, Last); \
        \
        template <class T, class Last> \
        auto trace_##Type(Here, T value, Last last) -> decltype( \
            trace_##Type(here, next(here, value), value) \
        ); \
        \
        Type next( \
            Here, decltype(trace_##Type( \
                here, next(here, here), here) \
            ) \
        ); \
    }

// pass all visitor classes in the chain to a type list and create a type alias
// using Type = TypeList<Visitor1, Visitor2, ...>
#define RPP_VISITOR_COLLECT(Type) \
    namespace visitor_chain { \
        template <class... Args> \
        rpp::TypeList<Args...> trace2_##Type(Here, rpp::VisitorTail, Args...); \
        \
        template <class T, class... Args> \
        auto trace2_##Type(Here, T value, Args... args) -> decltype( \
            trace2_##Type(here, next(here, value), args..., value) \
        ); \
        \
        using Type = \
            decltype(trace2_##Type( \
                here, next(here, here)) \
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
