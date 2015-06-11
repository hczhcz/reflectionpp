#include <type_traits>
#include "../reflection++/static_str.hpp"

namespace rpp {

    static_assert(str_index("abc", 1) == 'b', "");
    static_assert(str_index("abc", 3) == '\0', "");
    static_assert(str_index("abc", 5) == '\0', "");

    template <const char *str>
    struct SomeStrClass {};

    SomeStrClass<RPP_STATIC_STR("abc")> some_str_object;

}
