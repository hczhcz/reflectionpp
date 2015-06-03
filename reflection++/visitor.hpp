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
    Return visit(...) {
        return Return(); // notice: void is not constructable
    }

    Return into(...) {
        return Return(); // notice: void is not constructable
    }
};

}
