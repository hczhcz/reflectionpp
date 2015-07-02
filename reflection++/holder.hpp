#pragma once

#include "static_str.hpp"

namespace rpp {

template <class T>
using HoldType = decltype((*static_cast<T *>(nullptr))());

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

// generate a holder type
#define RPP_HOLDER_TYPE(...) \
    rpp::HolderType<__VA_ARGS__>
#define RPP_HOLDER_CONST(...) \
    rpp::HolderConst<decltype(__VA_ARGS__), __VA_ARGS__>
#define RPP_HOLDER_STR(...) \
    rpp::HolderConst<const char (&)[], RPP_STATIC_STR(__VA_ARGS__)>
#define RPP_HOLDER_LOCAL(...) \
    rpp::HolderLocal<__VA_ARGS__>
#define RPP_HOLDER_REF(...) \
    rpp::HolderRef<decltype(__VA_ARGS__), __VA_ARGS__>
#define RPP_HOLDER_DYNAMIC(...) \
    rpp::HolderDynamic<__VA_ARGS__>
#define RPP_HOLDER_MEMBER(Object, ...) \
    rpp::HolderMember< \
        decltype(rpp::realObject(&Object::__VA_ARGS__)), \
        decltype(Object::__VA_ARGS__), \
        &Object::__VA_ARGS__ \
    >

}
