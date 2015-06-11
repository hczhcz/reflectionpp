#pragma once

namespace rpp {

using rpp_strlen_t = unsigned;

constexpr rpp_strlen_t str_len(const char *str, rpp_strlen_t index = 0) {
    return str[index] == '\0' ? index : str_len(str, index + 1);
}

constexpr char str_index(const char *str, rpp_strlen_t index) {
    return index < str_len(str) ? str[index] : '\0';
}

// a compile-time string
template <char... c>
struct StaticStr {
    template <char tail>
    using Append = StaticStr<c..., tail>;

    static const char str[];

    // compile-time only
    StaticStr() = delete;
};

template <char... c>
const char StaticStr<c...>::str[]{c...};

#define RPP_STATIC_STR(Str) \
    (rpp::StaticStr<> \
        ::Append<rpp::str_index(Str, 0)>::Append<rpp::str_index(Str, 1)> \
        ::Append<rpp::str_index(Str, 2)>::Append<rpp::str_index(Str, 3)> \
        ::Append<rpp::str_index(Str, 4)>::Append<rpp::str_index(Str, 5)> \
        ::Append<rpp::str_index(Str, 6)>::Append<rpp::str_index(Str, 7)> \
        ::Append<rpp::str_index(Str, 8)>::Append<rpp::str_index(Str, 9)> \
        ::Append<rpp::str_index(Str, 10)>::Append<rpp::str_index(Str, 11)> \
        ::Append<rpp::str_index(Str, 12)>::Append<rpp::str_index(Str, 13)> \
        ::Append<rpp::str_index(Str, 14)>::Append<rpp::str_index(Str, 15)> \
        ::Append<rpp::str_index(Str, 16)>::Append<rpp::str_index(Str, 17)> \
        ::Append<rpp::str_index(Str, 18)>::Append<rpp::str_index(Str, 19)> \
        ::Append<rpp::str_index(Str, 20)>::Append<rpp::str_index(Str, 21)> \
        ::Append<rpp::str_index(Str, 22)>::Append<rpp::str_index(Str, 23)> \
        ::Append<rpp::str_index(Str, 24)>::Append<rpp::str_index(Str, 25)> \
        ::Append<rpp::str_index(Str, 26)>::Append<rpp::str_index(Str, 27)> \
        ::Append<rpp::str_index(Str, 28)>::Append<rpp::str_index(Str, 29)> \
        ::Append<rpp::str_index(Str, 30)>::Append<rpp::str_index(Str, 31)> \
        ::Append<rpp::str_index(Str, 32)>::Append<rpp::str_index(Str, 33)> \
        ::Append<rpp::str_index(Str, 34)>::Append<rpp::str_index(Str, 35)> \
        ::Append<rpp::str_index(Str, 36)>::Append<rpp::str_index(Str, 37)> \
        ::Append<rpp::str_index(Str, 38)>::Append<rpp::str_index(Str, 39)> \
        ::Append<rpp::str_index(Str, 40)>::Append<rpp::str_index(Str, 41)> \
        ::Append<rpp::str_index(Str, 42)>::Append<rpp::str_index(Str, 43)> \
        ::Append<rpp::str_index(Str, 44)>::Append<rpp::str_index(Str, 45)> \
        ::Append<rpp::str_index(Str, 46)>::Append<rpp::str_index(Str, 47)> \
        ::Append<rpp::str_index(Str, 48)>::Append<rpp::str_index(Str, 49)> \
        ::Append<rpp::str_index(Str, 50)>::Append<rpp::str_index(Str, 51)> \
        ::Append<rpp::str_index(Str, 52)>::Append<rpp::str_index(Str, 53)> \
        ::Append<rpp::str_index(Str, 54)>::Append<rpp::str_index(Str, 55)> \
        ::Append<rpp::str_index(Str, 56)>::Append<rpp::str_index(Str, 57)> \
        ::Append<rpp::str_index(Str, 58)>::Append<rpp::str_index(Str, 59)> \
        ::Append<rpp::str_index(Str, 60)>::Append<rpp::str_index(Str, 61)> \
        ::Append<rpp::str_index(Str, 62)>::Append<rpp::str_index(Str, 63)> \
        ::Append<'\0'>::str \
    )

}
