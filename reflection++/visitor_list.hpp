#pragma once

namespace rpp {

// a compile-time type list to contain visitor classes
template <class... Args>
struct VisitorList final {
    // compile-time only
    VisitorList() = delete;
};

}
