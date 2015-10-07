#pragma once

namespace rpp {

// get reference of a type
template <class T>
using RefCast = T &;

// get the actual object of a member pointer
template <class Object, class T>
Object memberToObject(T Object::*);

}
