#pragma once

#include "utility.hpp"

namespace rpp {

constexpr rpp_size_t strLen(const char *str, rpp_size_t index = 0) {
    return str[index] == 0 ? index : strLen(str, index + 1);
}

constexpr char strIndex(const char *str, rpp_size_t index) {
    return index < strLen(str) ? str[index] : 0;
}

// a compile-time string
template <char tail, char... c>
struct StaticStr final {
    template <char next>
    using Append = StaticStr<next, c..., tail>;

    // compile-time only
    StaticStr() = delete;
};

template <char... c>
struct StaticStr<0, c...> final {
    static const char str[];

    template <char next>
    using Append = StaticStr<0, c...>;

    // compile-time only
    StaticStr() = delete;
};

template <char... c>
const char StaticStr<0, c...>::str[]{c..., 0};

#define RPP_STATIC_STR(Str) \
    ( \
        rpp::StaticStr<rpp::strIndex((Str), 0)>::Append<rpp::strIndex((Str), 1)> \
            ::Append<rpp::strIndex((Str), 2)>::Append<rpp::strIndex((Str), 3)> \
            ::Append<rpp::strIndex((Str), 4)>::Append<rpp::strIndex((Str), 5)> \
            ::Append<rpp::strIndex((Str), 6)>::Append<rpp::strIndex((Str), 7)> \
            ::Append<rpp::strIndex((Str), 8)>::Append<rpp::strIndex((Str), 9)> \
            ::Append<rpp::strIndex((Str), 10)>::Append<rpp::strIndex((Str), 11)> \
            ::Append<rpp::strIndex((Str), 12)>::Append<rpp::strIndex((Str), 13)> \
            ::Append<rpp::strIndex((Str), 14)>::Append<rpp::strIndex((Str), 15)> \
            ::Append<rpp::strIndex((Str), 16)>::Append<rpp::strIndex((Str), 17)> \
            ::Append<rpp::strIndex((Str), 18)>::Append<rpp::strIndex((Str), 19)> \
            ::Append<rpp::strIndex((Str), 20)>::Append<rpp::strIndex((Str), 21)> \
            ::Append<rpp::strIndex((Str), 22)>::Append<rpp::strIndex((Str), 23)> \
            ::Append<rpp::strIndex((Str), 24)>::Append<rpp::strIndex((Str), 25)> \
            ::Append<rpp::strIndex((Str), 26)>::Append<rpp::strIndex((Str), 27)> \
            ::Append<rpp::strIndex((Str), 28)>::Append<rpp::strIndex((Str), 29)> \
            ::Append<rpp::strIndex((Str), 30)>::Append<rpp::strIndex((Str), 31)> \
            ::Append<rpp::strIndex((Str), 32)>::Append<rpp::strIndex((Str), 33)> \
            ::Append<rpp::strIndex((Str), 34)>::Append<rpp::strIndex((Str), 35)> \
            ::Append<rpp::strIndex((Str), 36)>::Append<rpp::strIndex((Str), 37)> \
            ::Append<rpp::strIndex((Str), 38)>::Append<rpp::strIndex((Str), 39)> \
            ::Append<rpp::strIndex((Str), 40)>::Append<rpp::strIndex((Str), 41)> \
            ::Append<rpp::strIndex((Str), 42)>::Append<rpp::strIndex((Str), 43)> \
            ::Append<rpp::strIndex((Str), 44)>::Append<rpp::strIndex((Str), 45)> \
            ::Append<rpp::strIndex((Str), 46)>::Append<rpp::strIndex((Str), 47)> \
            ::Append<rpp::strIndex((Str), 48)>::Append<rpp::strIndex((Str), 49)> \
            ::Append<rpp::strIndex((Str), 50)>::Append<rpp::strIndex((Str), 51)> \
            ::Append<rpp::strIndex((Str), 52)>::Append<rpp::strIndex((Str), 53)> \
            ::Append<rpp::strIndex((Str), 54)>::Append<rpp::strIndex((Str), 55)> \
            ::Append<rpp::strIndex((Str), 56)>::Append<rpp::strIndex((Str), 57)> \
            ::Append<rpp::strIndex((Str), 58)>::Append<rpp::strIndex((Str), 59)> \
            ::Append<rpp::strIndex((Str), 60)>::Append<rpp::strIndex((Str), 61)> \
            ::Append<rpp::strIndex((Str), 62)>::Append<rpp::strIndex((Str), 63)> \
            ::str \
    )

}
