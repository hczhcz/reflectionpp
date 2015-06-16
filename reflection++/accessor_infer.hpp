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

// set the default accessor to AccessorObject and bind members (from arguments)
#define RPP_ACCESSOR_BIND(Type, ...) \
    rpp::AccessorObjectWrap<rpp::TypeList<__VA_ARGS__>> accessor_infer(Type);

// set the default accessor to AccessorObject and bind members (pass a TypeList)
#define RPP_ACCESSOR_BIND_LIST(Type, Members) \
    rpp::AccessorObjectWrap<Members> accessor_infer(Type);

// get the default accessor
#define RPP_ACCESSOR_GET(Name, Value) \
    decltype( \
        accessor_infer((*static_cast<Value *>(nullptr))()) \
    )::Target<Name, Value>

// wrapper of RPP_HOLDER_REF
#define RPP_ACCESSOR_APPEND_REF(Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_REF(Value))>

// wrapper of RPP_HOLDER_MEMBER
#define RPP_ACCESSOR_APPEND_MEMBER(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_MEMBER(Object, Value))>

// call RPP_ACCESSOR_BIND and always use HolderMember
// some helper macros
#define RPP_ACCESSOR_BIND_OBJECT_ALSO , RPP_ACCESSOR_BIND_OBJECT_MEMBER),
#ifndef ___
    #define ___ RPP_ACCESSOR_BIND_OBJECT_ALSO
#endif
#define RPP_ACCESSOR_BIND_OBJECT_MEMBER_1(Method, Object, Member, Next) \
    Method(Object, Member) Next##_2(Method, Object
#define RPP_ACCESSOR_BIND_OBJECT_MEMBER_2(Method, Object, Member, Next) \
    Method(Object, Member) Next##_1(Method, Object
#define RPP_ACCESSOR_BIND_OBJECT_END_1(Method, Object) /* nothing */
#define RPP_ACCESSOR_BIND_OBJECT_END_2(Method, Object) /* nothing */
// the main macros
#define RPP_ACCESSOR_BIND_OBJECT(Object, Members) \
    rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_ACCESSOR_BIND_OBJECT_MEMBER_1( \
            RPP_ACCESSOR_APPEND_MEMBER, Object, Members, RPP_ACCESSOR_BIND_OBJECT_END \
        ) /* extra ")" */ ) \
    > accessor_infer(Object);

}
