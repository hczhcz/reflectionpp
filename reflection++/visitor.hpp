#pragma once

#include "utility.hpp"

namespace rpp {

// the base class of meta information visitors
template <class Return = void>
struct VisitorBase {
    using ReturnType = Return;
};

// visitors that ignore unknown values
template <class Return = void>
struct VisitorIgnoreBase: public VisitorBase<Return> {
    Return operator()(...) {
        static const Return value{};

        return value;
    }
};

template <>
struct VisitorIgnoreBase<void>: public VisitorBase<void> {
    void operator()(...) {}
};

}
