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
    static const char str[];

    // compile-time only
    StaticStr() = delete;
};

template <char... c>
const char StaticStr<c...>::str[]{c...};

#define RPP_STATIC_STR(Str) \
    (rpp::StaticStr< \
        rpp::str_index(Str, 0), rpp::str_index(Str, 1), \
        rpp::str_index(Str, 2), rpp::str_index(Str, 3), \
        rpp::str_index(Str, 4), rpp::str_index(Str, 5), \
        rpp::str_index(Str, 6), rpp::str_index(Str, 7), \
        rpp::str_index(Str, 8), rpp::str_index(Str, 9), \
        rpp::str_index(Str, 10), rpp::str_index(Str, 11), \
        rpp::str_index(Str, 12), rpp::str_index(Str, 13), \
        rpp::str_index(Str, 14), rpp::str_index(Str, 15), \
        rpp::str_index(Str, 16), rpp::str_index(Str, 17), \
        rpp::str_index(Str, 18), rpp::str_index(Str, 19), \
        rpp::str_index(Str, 20), rpp::str_index(Str, 21), \
        rpp::str_index(Str, 22), rpp::str_index(Str, 23), \
        rpp::str_index(Str, 24), rpp::str_index(Str, 25), \
        rpp::str_index(Str, 26), rpp::str_index(Str, 27), \
        rpp::str_index(Str, 28), rpp::str_index(Str, 29), \
        rpp::str_index(Str, 30), rpp::str_index(Str, 31), \
        rpp::str_index(Str, 32), rpp::str_index(Str, 33), \
        rpp::str_index(Str, 34), rpp::str_index(Str, 35), \
        rpp::str_index(Str, 36), rpp::str_index(Str, 37), \
        rpp::str_index(Str, 38), rpp::str_index(Str, 39), \
        rpp::str_index(Str, 40), rpp::str_index(Str, 41), \
        rpp::str_index(Str, 42), rpp::str_index(Str, 43), \
        rpp::str_index(Str, 44), rpp::str_index(Str, 45), \
        rpp::str_index(Str, 46), rpp::str_index(Str, 47), \
        rpp::str_index(Str, 48), rpp::str_index(Str, 49), \
        rpp::str_index(Str, 50), rpp::str_index(Str, 51), \
        rpp::str_index(Str, 52), rpp::str_index(Str, 53), \
        rpp::str_index(Str, 54), rpp::str_index(Str, 55), \
        rpp::str_index(Str, 56), rpp::str_index(Str, 57), \
        rpp::str_index(Str, 58), rpp::str_index(Str, 59), \
        rpp::str_index(Str, 60), rpp::str_index(Str, 61), \
        rpp::str_index(Str, 62), rpp::str_index(Str, 63), \
        '\0' \
    >::str)

}
