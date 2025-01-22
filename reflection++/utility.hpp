#pragma once

// TODO: include?
#define RPP_EXCEPTION_PRINT
#ifdef RPP_EXCEPTION_PRINT
    #include <iostream>
#endif

namespace rpp {

// type of size
using rpp_size_t = unsigned long;

// exception
struct Exception {
    // TODO
    Exception() {
        //
    }

    Exception(const char *file, const rpp_size_t line, const char *func) {
        #ifdef RPP_EXCEPTION_PRINT
            std::cerr << "ERROR: " << file << ":" << line << " " << func << std::endl;
        #endif
    }
};

// wrap a type (for macro arguments)
#define RPP_PASS(...) \
    decltype((static_cast<auto (*)() -> __VA_ARGS__>(nullptr))())

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
