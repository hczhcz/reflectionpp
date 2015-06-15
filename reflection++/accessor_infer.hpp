#pragma once

#include "accessor.hpp"

namespace rpp {

// a compile-time type to infer the default accessor of a type
template <class Name, class Value, class T = decltype((*static_cast<Value *>(nullptr))())>
struct AccessorDefault {
    using Target = AccessorSimple<Name, Value>;
};

// set the default accessor to AccessorObject and bind a TypeList
// under namespace rpp only
#define RPP_ACCESSOR_OBJECT_REG(Type, ...) \
    template <class Name, class Value> \
    struct AccessorDefault<Name, Value, Type &> { \
        using Target = AccessorObject<Name, Value, TypeList<__VA_ARGS__>>; \
    };

}
