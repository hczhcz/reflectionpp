#pragma once

namespace rpp {

// type of size
using rpp_size_t = unsigned long;

// exception
// TODO: more information
struct Exception {};

// wrap a type (for macro arguments)
#define RPP_PASS(...) \
    decltype((static_cast<auto (*)() -> __VA_ARGS__>(nullptr))())

// get reference of a type
template <class T>
using RefCast = T &;

// get the actual object of a member pointer
template <class Object, class T>
Object realObject(T Object::*);

// a compile-time type list to contain classes
template <class... Args>
struct TypeList final {
    template <class... Args2>
    using Push = TypeList<Args2..., Args...>;

    template <class... Args2>
    using Append = TypeList<Args..., Args2...>;

    template <template <class...> class Template>
    using Head = Template<Args...>;

    template <class T>
    using PushList = typename T::template Head<Push>;

    template <class T>
    using AppendList = typename T::template Head<Append>;

    // compile-time only
    TypeList() = delete;

    // for type inference
    static TypeList<Args...> instance();
};

}
