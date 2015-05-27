#pragma once

namespace rpp {

// the base class of meta information visitors
template <class Nothing = void>
struct VisitorBase {
    using ReturnValue = void;
};

// a compile-time type list to contain visitor classes
template <class... Args>
struct VisitorList final {
    // compile-time only
    VisitorList() = delete;
};

}
