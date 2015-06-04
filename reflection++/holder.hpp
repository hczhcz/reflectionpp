#pragma once

namespace rpp {

// data holders that hold constant data
template <class T, T value>
struct HolderConst {
    HolderConst() {}

    const T operator()() {
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
struct HolderStatic {
    HolderStatic() {}

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

    HolderMember(Object &_object): object{_object} {}

    T &operator()() {
        return object.*member;
    }
};

// // data holders that use "this" pointer directly
// template <class T>
// struct HolderThis: public T {
//     using RealType = T;
//     using T::T;

//     T &operator()() {
//         return *this;
//     }
// };

// // data holders point to a specified member of "this" pointer
// template <class Base, class T, T Base::RealType::*member>
// struct HolderThisMember: public Base {
//     using Base::Base;

//     T &operator()() {
//         return this->*member;
//     }
// };

}
