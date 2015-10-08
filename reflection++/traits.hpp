#pragma once

namespace rpp {

// get a reference to a type
template <class T>
using GetRef = T &;

// get the return type of a function
template <class T>
using GetReturn = decltype((*static_cast<T *>(nullptr))());

// generate an error if the value is not true
template <bool value>
using Ensure = decltype(static_cast<void>(sizeof(char [value ? 1 : -1])));

// get the actual object of a member pointer
template <class Object, class T>
Object memberToObject(T Object::*);

}
