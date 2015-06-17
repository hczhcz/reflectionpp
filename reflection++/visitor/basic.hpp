#pragma once

#include <utility>
#include <typeinfo>
#include "../visitor_chain.hpp"

namespace rpp {

struct VisitorType: public VisitorBase<const std::type_info &> {
    template <class T>
    ReturnType visit(T &value) {
        return typeid(value);
    }

    template <class Accessor>
    ReturnType into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

RPP_VISITOR_REG(VisitorType)

struct VisitorSize: public VisitorBase<std::size_t> {
    template <class T>
    ReturnType visit(T &value) {
        return sizeof(value);
    }

    template <class Accessor>
    ReturnType into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

RPP_VISITOR_REG(VisitorSize)

struct VisitorPointer: public VisitorBase<const void *> {
    template <class T>
    ReturnType visit(T &value) {
        return static_cast<ReturnType>(&value);
    }

    template <class Accessor>
    ReturnType into(Accessor &accessor) {
        return accessor.doObjectVisit(*this);
    }
};

RPP_VISITOR_REG(VisitorPointer)

}
