#pragma once

namespace rpp {

// the base class of data accessors
template <class Nothing = void>
struct AccessorBase {};

template <class T>
struct AccessorLocal: public AccessorBase<> {
    T value;

    AccessorLocal(T &&_value): value(_value) {}
    AccessorLocal(const T &_value): value(_value) {}

    T &access() {
        return value;
    }
};

template <class T, T &value>
struct AccessorStatic: public AccessorBase<> {
    T &access() {
        return value;
    }
};

template <class T>
struct AccessorDynamic: public AccessorBase<> {
    T &value;

    AccessorDynamic(T &_value): value(_value) {}

    T &access() {
        return value;
    }
};

template <class Object, class T, T &Object::member>
struct AccessorMember: public AccessorBase<> {
    Object &object;

    AccessorMember(T &_object): object(_object) {}

    T &access() {
        return object.member;
    }
};

}
