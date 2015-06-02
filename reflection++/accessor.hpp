#pragma once

namespace rpp {

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

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(value);
    }
};

// data accessors point to a specified variable
template <class T, T &value>
struct AccessorStatic: public AccessorBase<> {
    AccessorStatic() {}

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

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.visit(*(object.member));
    }
};

// TODO: write comments
template <class Base, class... Members>
struct AccessorObjectHelper;

template <class Base>
struct AccessorObjectHelper<Base>: public Base {
    using Base::Base;

    static constexpr auto size = 0;
};

template <class Base, class Member, class... Args>
struct AccessorObjectHelper<
    Base, Member, Args...
>: public AccessorObjectHelper<Base, Args...> {
    using AccessorObjectHelper<Base, Args...>::AccessorObjectHelper;

    static constexpr auto size = 1 + sizeof...(Args);
};

// TODO: write comments
template <class Base, class... Members>
struct AccessorObject: public AccessorBase<> {
    AccessorObjectHelper<Base, Members...> self;

    AccessorObject(decltype(self) &&_self): self{_self} {}

    template <class Visitor>
    typename Visitor::ReturnValue doRealVisit(Visitor &visitor) {
        return visitor.into(self);
    }
};

}
