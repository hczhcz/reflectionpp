#pragma once

#include <typeinfo>

#include "../visitor_chain.hpp"
#include "../accessor.hpp"

namespace rpp {

// get type_info object of a type
template <bool dynamic = true>
struct VisitorType: public VisitorBase<const std::type_info &> {
    template <class T>
    ReturnType visit(T &value) {
        if (dynamic) {
            return typeid(value);
        } else {
            return typeid(T);
        }
    }

    template <class T>
    ReturnType visit(const HolderType<T> &) {
        return typeid(T);
    }

    template <class Accessor>
    ReturnType operator()(Accessor &accessor) {
        return visit(accessor());
    }
};

RPP_VISITOR_REG(VisitorType<true>)
RPP_VISITOR_REG(VisitorType<false>)

// get size of a type
template <bool dynamic = true>
struct VisitorSize: public VisitorBase<rpp_size_t> {
    template <class T>
    ReturnType visit(T &value) {
        if (dynamic) {
            return sizeof(value);
        } else {
            return sizeof(T);
        }
    }

    template <class T>
    ReturnType visit(const HolderType<T> &) {
        return sizeof(T);
    }

    template <class Accessor>
    ReturnType operator()(Accessor &accessor) {
        return visit(accessor());
    }
};

RPP_VISITOR_REG(VisitorSize<true>)
RPP_VISITOR_REG(VisitorSize<false>)

// get pointer of a value
template <class Pointer>
struct VisitorPointer: public VisitorBase<Pointer *> {
    Pointer *visit(...) {
        throw Exception{};
    }

    template <class T>
    auto visit(T &value) -> decltype(static_cast<Pointer *>(&value)) {
        return static_cast<Pointer *>(&value);
    }

    template <class Accessor>
    Pointer *operator()(Accessor &accessor) {
        return visit(accessor());
    }
};

RPP_VISITOR_REG(VisitorPointer<void>)
RPP_VISITOR_REG(VisitorPointer<const void>)

}
