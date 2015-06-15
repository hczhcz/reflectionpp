#pragma once

#include "accessor.hpp"

namespace rpp {

// a compile-time wraper class of AccessorSimple
struct AccessorSimpleWrap {
    template <class Name, class Value>
    using Target = AccessorSimple<Name, Value>;

    // compile-time only
    AccessorSimpleWrap() = delete;
};

// a compile-time wraper class of AccessorObject
template <class Members>
struct AccessorObjectWrap {
    template <class Name, class Value>
    using Target = AccessorObject<Name, Value, Members>;

    // compile-time only
    AccessorObjectWrap() = delete;
};

// enable accessor_infer function in the current namespace
#define RPP_ACCESSOR_INFER_INIT() \
    /* an abstract function to infer the default accessor of a type */ \
    rpp::AccessorSimpleWrap accessor_infer(...);

RPP_ACCESSOR_INFER_INIT()

// set the default accessor to AccessorObject and bind a TypeList
#define RPP_ACCESSOR_INFER_BIND(Type, ...) \
    rpp::AccessorObjectWrap<rpp::TypeList<__VA_ARGS__>> accessor_infer(Type);

// get the default accessor
#define RPP_ACCESSOR_INFER_GET(Name, Value) \
    decltype( \
        accessor_infer((*static_cast<Value *>(nullptr))()) \
    )::Target<Name, Value>

}
