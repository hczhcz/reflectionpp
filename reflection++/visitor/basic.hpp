#pragma once

#include <typeinfo>
#include "../visitor_chain.hpp"
#include "../accessor.hpp"

namespace rpp {

// get type_info object of a type
// template <class Nothing = void>
struct VisitorType: public VisitorBase<const std::type_info &> {
    template <class T>
    ReturnType visit(T &value) {
        return typeid(value);
    }

    template <class T>
    ReturnType visit(const HolderType<T> &) {
        return typeid(T);
    }

    template <class Accessor>
    ReturnType into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

RPP_VISITOR_REG(VisitorType)

// get size of a type
// template <class Nothing = void>
struct VisitorSize: public VisitorBase<rpp_size_t> {
    template <class T>
    ReturnType visit(T &value) {
        return sizeof(value);
    }

    template <class T>
    ReturnType visit(const HolderType<T> &) {
        return sizeof(T);
    }

    template <class Accessor>
    ReturnType into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

RPP_VISITOR_REG(VisitorSize)

// get pointer of a value
template <class Pointer>
struct VisitorPointer: public VisitorBase<Pointer *> {
    template <class T>
    Pointer *visit(T &value) {
        return static_cast<Pointer *>(&value);
    }

    template <class Accessor>
    Pointer *into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

using VisitorPointerRaw = VisitorPointer<void>;
using VisitorPointerRawConst = VisitorPointer<const void>;

RPP_VISITOR_REG(VisitorPointerRaw)
RPP_VISITOR_REG(VisitorPointerRawConst)

}
