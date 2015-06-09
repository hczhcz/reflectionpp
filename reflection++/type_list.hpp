#pragma once

namespace rpp {

// a compile-time type list to contain classes
template <class... Args>
struct TypeList final {
    template <class T>
    using Push = TypeList<T, Args...>;

    template <template <class...> class Template>
    using Head = Template<Args...>;

    // compile-time only
    TypeList() = delete;
};

}
