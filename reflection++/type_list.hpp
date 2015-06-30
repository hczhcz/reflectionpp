#pragma once

namespace rpp {

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
