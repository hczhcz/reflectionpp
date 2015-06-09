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
    StaticStr< \
        str_index(Str, 0), str_index(Str, 1), str_index(Str, 2), str_index(Str, 3), \
        str_index(Str, 4), str_index(Str, 5), str_index(Str, 6), str_index(Str, 7), \
        str_index(Str, 8), str_index(Str, 9), str_index(Str, 10), str_index(Str, 11), \
        str_index(Str, 12), str_index(Str, 13), str_index(Str, 14), str_index(Str, 15), \
        str_index(Str, 16), str_index(Str, 17), str_index(Str, 18), str_index(Str, 19), \
        str_index(Str, 20), str_index(Str, 21), str_index(Str, 22), str_index(Str, 23), \
        str_index(Str, 24), str_index(Str, 25), str_index(Str, 26), str_index(Str, 27), \
        str_index(Str, 28), str_index(Str, 29), str_index(Str, 30), str_index(Str, 31), \
        str_index(Str, 32), str_index(Str, 33), str_index(Str, 34), str_index(Str, 35), \
        str_index(Str, 36), str_index(Str, 37), str_index(Str, 38), str_index(Str, 39), \
        str_index(Str, 40), str_index(Str, 41), str_index(Str, 42), str_index(Str, 43), \
        str_index(Str, 44), str_index(Str, 45), str_index(Str, 46), str_index(Str, 47), \
        str_index(Str, 48), str_index(Str, 49), str_index(Str, 50), str_index(Str, 51), \
        str_index(Str, 52), str_index(Str, 53), str_index(Str, 54), str_index(Str, 55), \
        str_index(Str, 56), str_index(Str, 57), str_index(Str, 58), str_index(Str, 59), \
        str_index(Str, 60), str_index(Str, 61), str_index(Str, 62), str_index(Str, 63), \
        '\0' \
    >::str

}

// ======== Usage example ========

#ifdef RPP_DEBUG

    namespace rpp {

        static_assert(str_index("abc", 1) == 'b', "");
        static_assert(str_index("abc", 3) == '\0', "");
        static_assert(str_index("abc", 5) == '\0', "");

    }

#endif
