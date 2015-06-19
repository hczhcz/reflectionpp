#pragma once

#include "static_str.hpp"

namespace rpp {

// data holders that hold types
template <class T>
struct HolderType {
    HolderType() {}

    const HolderType<T> &operator()() {
        return *this;
    }
};

// data holders that hold data as a static variable in a function
// allow constant only
template <class T, T _value>
struct HolderConst {
    HolderConst() {}

    const T &operator()() {
        static const T value{_value};

        return value;
    }
};

// data holders that hold data by themselves
template <class T>
struct HolderLocal {
    T value;

    HolderLocal(T &&_value): value(static_cast<T &&>(_value)) {}
    HolderLocal(const T &_value): value(_value) {}
    // HolderLocal(T &&_value): value{static_cast<T &&>(_value)} {}
    // HolderLocal(const T &_value): value{_value} {}

    T &operator()() {
        return value;
    }
};

// data holders point to a specified variable
template <class T, T &value>
struct HolderRef {
    HolderRef() {}

    T &operator()() {
        return value;
    }
};

// data holders point to a run-time variable
template <class T>
struct HolderDynamic {
    T &value;

    HolderDynamic(T &_value): value{_value} {}

    T &operator()() {
        return value;
    }
};

// data holders point to a specified member of a class
template <class Object, class T, T Object::*member>
struct HolderMember {
    Object &object;

    HolderMember(
        Object &_object
    ): object{_object} {}

    T &operator()() {
        return object.*member;
    }
};

#define RPP_HOLDER_TYPE(Type) \
    rpp::HolderType<Type>
#define RPP_HOLDER_CONST(Value) \
    rpp::HolderConst<decltype(Value), Value>
#define RPP_HOLDER_STR(Str) \
    rpp::HolderConst<const char (&)[], RPP_STATIC_STR(Str)>
#define RPP_HOLDER_LOCAL(Type) \
    rpp::HolderLocal<Type>
#define RPP_HOLDER_REF(Value) \
    rpp::HolderRef<decltype(Value), Value>
#define RPP_HOLDER_DYNAMIC(Type) \
    rpp::HolderDynamic<Type>
#define RPP_HOLDER_MEMBER(Object, Value) \
    rpp::HolderMember<Object, decltype(Object::Value), &Object::Value>

}
