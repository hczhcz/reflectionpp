#pragma once

namespace rpp {

struct {} visitor_head;

struct VisitorBase {};

struct VisitorTail {};

VisitorTail visitor_next(...);

#define VISITOR_REG(Type) \
    template <class T, class Last> \
    auto visitor_trace_##Type(T value, Last last) -> decltype( \
        VisitorBase(value), \
        visitor_trace_##Type(visitor_next(value), value) \
    ); \
    template <class Last> \
    Last visitor_trace_##Type(VisitorTail, Last); \
    Type visitor_next( \
        decltype(visitor_trace_##Type(visitor_next(visitor_head), visitor_head)) \
    );

#define VISITOR_COLLECT(Type, Wrapper) \
    template <class T, class... Args> \
    auto visitor_trace_##Type(T value, Args... args) -> decltype( \
        VisitorBase(value), \
        visitor_trace_##Type(visitor_next(value), args..., value) \
    ); \
    template <class... Args> \
    Wrapper<Args...> visitor_trace_##Type(VisitorTail, Args...); \
    using Type = \
        decltype(visitor_trace_##Type(visitor_next(visitor_head)));

}
