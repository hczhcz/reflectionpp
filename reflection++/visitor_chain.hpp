#pragma once

#include "visitor.hpp"

namespace rpp {

// the abstract tail of the visitor chain
struct VisitorTail final {
    // compile-time only
    VisitorTail() = delete;
};

// register a visitor class into the visitor chain
// helper macros
#define RPP_VISITOR_REG_UNIQUE_2(Counter, ...) \
    namespace visitor_chain { \
        template <class Last> \
        Last trace_##Counter(Here, rpp::VisitorTail &, Last &); \
        \
        template <class T, class Last> \
        auto trace_##Counter(Here, T &value, Last &last) -> decltype( \
            trace_##Counter(here, next(here, value), value) \
        ); \
        \
        rpp::RefCast<__VA_ARGS__> next( \
            Here, \
            decltype( \
                trace_##Counter(here, next(here, here), here) \
            ) & \
        ); \
    }
#define RPP_VISITOR_REG_UNIQUE_1(...) \
    RPP_VISITOR_REG_UNIQUE_2(__VA_ARGS__)
// the main macro
#define RPP_VISITOR_REG(...) \
    RPP_VISITOR_REG_UNIQUE_1(__COUNTER__, __VA_ARGS__)

// pass all visitor classes in the chain to a type list and create a type alias
// using Type = TypeList<Visitor1, Visitor2, ...>
#define RPP_VISITOR_COLLECT(Type) \
    namespace visitor_chain { \
        template <class... Args> \
        rpp::TypeList<Args...> trace2_##Type(Here, rpp::VisitorTail &, rpp::TypeList<Args...>); \
        \
        template <class T, class... Args> \
        auto trace2_##Type(Here, T &value, rpp::TypeList<Args...>) -> decltype( \
            trace2_##Type( \
                here, \
                next(here, value), \
                rpp::TypeList<Args...>::template Append<T>::instance() \
            ) \
        ); \
        \
        using Type = \
            decltype( \
                trace2_##Type( \
                    here, \
                    next(here, here), \
                    rpp::TypeList<>::instance() \
                ) \
            ); \
    } \
    \
    using Type = visitor_chain::Type;

// enable the visitor chain in the current namespace
#define RPP_VISITOR_CHAIN_INIT() \
    namespace visitor_chain { \
        /* an empty object to control argument-dependent lookup */ \
        /* and provide a head of the visitor chain */ \
        static constexpr struct Here final { \
            Here() = delete; \
        } here{}; \
        \
        /* an abstract function to infer the next member of the visitor chain */ \
        rpp::VisitorTail &next(...); \
        \
        /* call next with VisitorTail is not allowed */ \
        void next(Here, rpp::VisitorTail &) = delete; \
    }

RPP_VISITOR_CHAIN_INIT()

}
