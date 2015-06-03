#pragma once

#include "accessor.hpp"
#include "accessor_object.hpp"

namespace rpp {

// data accessors that hold data by themselves
template <class T>
struct AccessorLocal: public AccessorBase<> {
    T value;

    AccessorLocal(T &&_value): value(static_cast<T &&>(_value)) {}
    AccessorLocal(const T &_value): value(_value) {}
    // AccessorLocal(T &&_value): value{static_cast<T &&>(_value)} {}
    // AccessorLocal(const T &_value): value{_value} {}

    T &get() {
        return value;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(value);
    }
};

// data accessors point to a specified variable
template <class T, T &value>
struct AccessorStatic: public AccessorBase<> {
    AccessorStatic() {}

    T &get() {
        return value;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(value);
    }
};

// data accessors point to a run-time variable
template <class T>
struct AccessorDynamic: public AccessorBase<> {
    T &value;

    AccessorDynamic(T &_value): value{_value} {}

    T &get() {
        return value;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(value);
    }
};

// data accessors point to a specified member of a class
template <class Object, class T, T Object::*member>
struct AccessorMember: public AccessorBase<> {
    Object &object;

    AccessorMember(Object &_object): object{_object} {}

    T &get() {
        return object.*member;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(object.*member);
    }
};

// data accessors associated with members
template <class Self, class... Args>
struct AccessorObject: protected AccessorObjectHelper<Self, Args...> {
    using AccessorObjectHelper<Self, Args...>::AccessorObjectHelper;

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.into(
            *static_cast<AccessorObjectHelper<Self, Args...> *>(this)
        );
    }
};

}
