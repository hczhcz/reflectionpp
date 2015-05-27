#pragma once

#include "visitor.hpp"

namespace rpp {

struct {} visitor_head;

struct VisitorTail {};

VisitorTail visitor_next(...);

// register a visitor class
#define VISITOR_REG(Type) \
    template <class T, class Last> \
    auto visitor_trace_##Type(T value, Last last) -> decltype( \
        VisitorBase<>(value), \
        visitor_trace_##Type(visitor_next(value), value) \
    ); \
    \
    template <class Last> \
    Last visitor_trace_##Type(VisitorTail, Last); \
    \
    Type visitor_next( \
        decltype(visitor_trace_##Type(visitor_next(visitor_head), visitor_head)) \
    );

// pass all visitor classes to VisitorList and get a type alias
// using Type = VisitorList<Visitor1, Visitor2, ...>
#define VISITOR_COLLECT(Type) \
    template <class T, class... Args> \
    auto visitor_trace2_##Type(T value, Args... args) -> decltype( \
        VisitorBase<>(value), \
        visitor_trace2_##Type(visitor_next(value), args..., value) \
    ); \
    \
    template <class... Args> \
    VisitorList<Args...> visitor_trace2_##Type(VisitorTail, Args...); \
    \
    using Type = \
        decltype(visitor_trace2_##Type(visitor_next(visitor_head)));

// ======== Usage example ========

// struct V1: public VisitorBase<> {};
// struct V2: public VisitorBase<> {};
// namespace some_namespace {
//     struct V3: public VisitorBase<> {};
// }
// using V3 = some_namespace::V3;

// VISITOR_REG(V1)
// VISITOR_REG(V2)
// VISITOR_REG(V3)

// VISITOR_COLLECT(VAll)

// VAll all = "make_error"; // show the type of VAll

}
