#pragma once

namespace rpp {

using rpp_size_t = unsigned;

// the base class of data accessors
template <class Nothing = void>
struct AccessorBase {};

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

// data accessors associated with members (mixin)
template <class Base, class... Members>
struct AccessorObject;

template <class Base>
struct AccessorObject<Base>: protected Base {
    using Base::Base;

    rpp_size_t size() {
        return 0;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.into(*this);
    }

    template <class Visitor>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor, rpp_size_t index) {
        (void) visitor;
        (void) index;

        throw 1; // TODO
    }
};

template <class Base, class Member, class... Args>
struct AccessorObject<
    Base, Member, Args...
>: protected AccessorObject<Base, Args...> {
    using AccessorObject<Base, Args...>::AccessorObject;

    rpp_size_t size() {
        return 1 + sizeof...(Args);
    }

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.into(*this);
    }

    template <class Visitor>
    typename Visitor::ReturnValue doObjectVisit(Visitor &visitor) {
        return Base::doRealVisit(visitor);
    }

    template <class Visitor, rpp_size_t index>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor) {
        if (index == 0) {
            Member member{Base::get()};
            return member.doRealVisit(visitor);
        } else {
            AccessorObject<Base, Args...>
                ::template doMemberVisit<Visitor, index - 1>(visitor);
        }
    }

    template <class Visitor>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor, rpp_size_t index) {
        if (index == 0) {
            Member member{Base::get()};
            return member.doRealVisit(visitor);
        } else {
            AccessorObject<Base, Args...>
                ::template doMemberVisit<Visitor>(visitor, index - 1);
        }
    }
};

}
