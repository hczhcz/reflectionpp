#pragma once

namespace rpp {

// a compile-time type list to contain classes
template <class... Args>
struct TypeList final {
    // compile-time only
    TypeList() = delete;
};

}
