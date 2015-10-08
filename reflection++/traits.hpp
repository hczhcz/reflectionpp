#pragma once

namespace rpp {

// get a reference to a type
template <class T>
using GetRef = T &;

// get the actual object of a member pointer
template <class Object, class T>
Object memberToObject(T Object::*);

}
