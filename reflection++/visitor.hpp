#pragma once

namespace rpp {

// the base class of meta information visitors
template <class Nothing = void>
struct VisitorBase {
    using ReturnValue = void;
};

// visitors that return values
template <class Return = void>
struct VisitorReturnBase: public VisitorBase<> {
    using ReturnValue = Return;
};

// visitors that ignore unknown values
template <class Return = void>
struct VisitorIgnoreBase: public VisitorReturnBase<Return> {
    Return visit(...) {
        return Return();
    }
};

// a compile-time type list to contain visitor classes
template <class... Args>
struct VisitorList final {
    // compile-time only
    VisitorList() = delete;
};

}
