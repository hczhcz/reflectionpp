#include <type_traits>
#include "../reflection++/static_str.hpp"

namespace rpp_another_namespace {

static_assert(rpp::str_index("abc", 1) == 'b', "");
static_assert(rpp::str_index("abc", 3) == '\0', "");
static_assert(rpp::str_index("abc", 5) == '\0', "");

template <const char *str>
struct SomeStrClass {};

SomeStrClass<RPP_STATIC_STR("abc")> some_str_object;

static_assert(std::is_same<
    decltype(some_str_object),
    SomeStrClass<rpp::StaticStr<
        0, 'a', 'b', 'c'
    >::str>
>(), "");

}
