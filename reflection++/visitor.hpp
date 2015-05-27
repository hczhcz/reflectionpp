#pragma once

namespace rpp {

struct VisitorBase {};

struct VisitorTail {};

VisitorTail visitor_next(...);

#define VISITOR_REG(Type) \
    template <class T> \
    auto visitor_trace_##Type(T value) -> decltype( \
        VisitorBase(visitor_next(value)), \
        visitor_trace_##Type(visitor_next(value)) \
    ); \
    template <class T> \
    auto visitor_trace_##Type(T value) -> decltype( \
        VisitorTail(visitor_next(value)), \
        value \
    ); \
    Type visitor_next(decltype(visitor_trace_##Type(nullptr)));

}
