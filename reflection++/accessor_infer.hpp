#pragma once

#include "accessor.hpp"

namespace rpp {

// a compile-time wraper class of AccessorSimple
template <class Nothing = void>
struct AccessorSimpleWrap {
    template <class Name, class Value>
    using Accessor = AccessorSimple<
        Name,
        Value
    >;

    // compile-time only
    AccessorSimpleWrap() = delete;
};

// a compile-time wraper class of AccessorObject
template <class Members>
struct AccessorObjectWrap {
    using List = Members;

    template <class Name, class Value>
    using Accessor = AccessorObject<
        Name,
        Value,
        Members
    >;

    // compile-time only
    AccessorObjectWrap() = delete;
};

// a compile-time wraper class of AccessorDynamic
template <class Member>
struct AccessorDynamicWrap {
    template <class Name, class Value>
    using Accessor = AccessorDynamic<
        Name,
        Value,
        Member
    >;

    // compile-time only
    AccessorDynamicWrap() = delete;
};

// // set the default accessor to AccessorObject and bind members (pass a TypeList)
// #define RPP_ACCESSOR_BIND_OBJECT(Type, Members) \
//     template <> \
//     struct AccessorInfer<Type &>: public rpp::AccessorObjectWrap<Members> {};

// // set the default accessor to AccessorDynamic and bind the (abstract) member
// #define RPP_ACCESSOR_BIND_DYNAMIC(Type, Member) \
//     template <> \
//     struct AccessorInfer<Type &>: public rpp::AccessorDynamicWrap<Member> {};

// get the default accessor
#define RPP_ACCESSOR_GET(Name, Value) \
    AccessorInfer< \
        decltype((*static_cast<Value *>(nullptr))()) \
    >::Accessor<Name, Value>

// enable AccessorInfer in the current namespace
#define RPP_ACCESSOR_INFER_INIT() \
    /* an abstract function to infer the default accessor of a type */ \
    template <class T> \
    struct AccessorInfer final: public rpp::AccessorSimpleWrap<> { \
        /* compile-time only */ \
        AccessorInfer() = delete; \
    };

RPP_ACCESSOR_INFER_INIT()

// append an accessor to a TypeList
#define RPP_ACCESSOR_APPEND_ACCESSOR(Object, Accessor) \
    ::Append<Accessor>

// append an accessor to a TypeList (using default accessors)
// wrapper of RPP_HOLDER macros
// notice: LOCAL and DYNAMIC means type casting
#define RPP_ACCESSOR_APPEND_TYPE(Object, Type) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Type), RPP_HOLDER_TYPE(Object::Type))>
#define RPP_ACCESSOR_APPEND_CONST(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_CONST(Object::Value))>
#define RPP_ACCESSOR_APPEND_LOCAL(Object, Type) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Type), RPP_HOLDER_LOCAL(Type))>
#define RPP_ACCESSOR_APPEND_REF(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_REF(Object::Value))>
#define RPP_ACCESSOR_APPEND_DYNAMIC(Object, Type) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Type), RPP_HOLDER_DYNAMIC(Type))>
#define RPP_ACCESSOR_APPEND_MEMBER(Object, Value) \
    ::Append<RPP_ACCESSOR_GET(RPP_HOLDER_STR(#Value), RPP_HOLDER_MEMBER(Object, Value))>

// append an accessor wraps a base type to a TypeList
#define RPP_ACCESSOR_APPEND_BASE(Derived, Base) \
    RPP_ACCESSOR_APPEND_DYNAMIC(Derived, Base)

// append accessors of a base type (fetched from AccessorObjectWrap) to a TypeList
#define RPP_ACCESSOR_APPEND_BASE_INLINE(Derived, Base) \
    ::AppendList<AccessorInfer< \
        decltype(*static_cast<Base *>(nullptr)) \
    >::List>

// build a TypeList of accessors
#define RPP_ACCESSOR_LIST_ITEM_IMPL(Member, Method, ...) \
    , RPP_ACCESSOR_LIST), RPP_ACCESSOR_APPEND_##Method, Member
#define RPP_ACCESSOR_LIST_ITEM(...) \
    RPP_ACCESSOR_LIST_ITEM_IMPL(__VA_ARGS__, MEMBER, )
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

// a more friendly version of RPP_ACCESSOR_BIND_OBJECT
#define RPP_TYPE_OBJECT(Object, Members) \
    template <> \
    struct AccessorInfer<Object &>: public rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_ACCESSOR_LIST_BEGIN( \
            Object \
            Members \
            , RPP_ACCESSOR_LIST_END) \
        ) \
    > {};

// a more friendly version of RPP_ACCESSOR_BIND_DYNAMIC
#define RPP_TYPE_DYNAMIC(Object, Member) \
    template <> \
    struct AccessorInfer<Object &>: public rpp::AccessorDynamicWrap< \
        RPP_ACCESSOR_GET(RPP_HOLDER_STR("member"), RPP_HOLDER_DYNAMIC(Member)) \
    > {};

}
