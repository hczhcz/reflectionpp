#pragma once

#include "accessor.hpp"

namespace rpp {

// a compile-time wraper class of AccessorSimple
struct AccessorSimpleWrap {
    template <class Name, class Value>
    using Build = AccessorSimple<Name, Value>;

    // compile-time only
    AccessorSimpleWrap() = delete;
};

// a compile-time wraper class of AccessorObject
template <class Members>
struct AccessorObjectWrap {
    using List = Members;

    template <class Name, class Value>
    using Build = AccessorObject<Name, Value, Members>;

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
    )::Build<Name, Value>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_TYPE
#define RPP_ACCESSOR_APPEND_TYPE(Object, Type) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Type), RPP_HOLDER_TYPE(Object::Type))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_CONST
#define RPP_ACCESSOR_APPEND_CONST(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_CONST(Object::Value))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_REF
#define RPP_ACCESSOR_APPEND_REF(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_REF(Object::Value))>

// append an accessor to a TypeList
// wrapper of RPP_HOLDER_MEMBER
#define RPP_ACCESSOR_APPEND_MEMBER(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_MEMBER(Object, Value))>

// append accessors fetched from AccessorObjectWrap to a TypeList
#define RPP_ACCESSOR_APPEND_INFER(Derived, Base) \
    ::AppendList<decltype( \
        accessor_infer((*static_cast<Base *>(nullptr))()) \
    )::List>

// build a TypeList of accessors
// some helper macros
#define RPP_ACCESSOR_LIST_ITEM(Method, Member) \
    , RPP_ACCESSOR_LIST), RPP_ACCESSOR_APPEND_##Method, Member
#ifndef __
    #define __ RPP_ACCESSOR_LIST_ITEM
#endif
#define RPP_ACCESSOR_LIST_BEGIN(Object, Next) \
    Next##_1(Object
#define RPP_ACCESSOR_LIST_1(Object, Method, Member, Next) \
    Method(Object, Member) Next##_2(Object
#define RPP_ACCESSOR_LIST_2(Object, Method, Member, Next) \
    Method(Object, Member) Next##_1(Object
#define RPP_ACCESSOR_LIST_END_1(...) /* nothing */
#define RPP_ACCESSOR_LIST_END_2(...) /* nothing */
// the main macro
#define RPP_ACCESSOR_BIND_OBJECT(Object, Members) \
    rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_ACCESSOR_LIST_BEGIN( \
            Object \
            Members \
            , RPP_ACCESSOR_LIST_END) \
        ) \
    > accessor_infer(Object);

}
