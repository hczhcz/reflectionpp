#pragma once

namespace rpp {

// the base class of meta information visitors
template <class Return = void>
struct VisitorBase {
    using ReturnType = Return;
};

// visitors that ignore unknown values
template <class Return = void>
struct VisitorIgnoreBase: public VisitorBase<Return> {
    Return visit(...) { // notice: not necessary
        static const Return value{};

        return value;
    }

    Return operator()(...) {
        static const Return value{};

        return value;
    }
};

template <>
struct VisitorIgnoreBase<void>: public VisitorBase<void> {
    void visit(...) {} // notice: not necessary

    void operator()(...) {}
};

}
