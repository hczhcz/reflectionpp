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

// get the default accessor
#define RPP_ACCESSOR(Name, Value) \
    typename AccessorInfer< \
        rpp::HoldType<Value> \
    >::template Accessor<Name, Value>

// get the default accessor, bind the holder's type and the name
#define RPP_ACCESSOR_AS(Mode, Name, ...) \
    RPP_ACCESSOR(RPP_HOLDER_STR(Name), RPP_HOLDER_##Mode(__VA_ARGS__))

// enable AccessorInfer in the current namespace
#define RPP_ACCESSOR_INFER_INIT() \
    /* an abstract function to infer the default accessor of a type */ \
    template <class T> \
    struct AccessorInfer final: public rpp::AccessorSimpleWrap<> { \
        /* compile-time only */ \
        AccessorInfer() = delete; \
    };

RPP_ACCESSOR_INFER_INIT()

// helper macros begin

    // append an accessor to a TypeList
    #define RPP_ACCESSOR_APPEND_ACCESSOR(Object, Accessor) \
        ::Append<Accessor>
    // append an accessor to a TypeList (using default accessors)
    // wrappers of RPP_ACCESSOR_AS
    // notice: LOCAL or DYNAMIC as member means type casting
    #define RPP_ACCESSOR_APPEND_TYPE(Object, Type) \
        ::Append<RPP_ACCESSOR_AS(TYPE, #Type, Object::Type)>
    #define RPP_ACCESSOR_APPEND_CONST(Object, Value) \
        ::Append<RPP_ACCESSOR_AS(CONST, #Value, Object::Value)>
    #define RPP_ACCESSOR_APPEND_LOCAL(Object, Type) \
        ::Append<RPP_ACCESSOR_AS(LOCAL, #Type, Type)>
    #define RPP_ACCESSOR_APPEND_REF(Object, Value) \
        ::Append<RPP_ACCESSOR_AS(REF, #Value, Object::Value)>
    #define RPP_ACCESSOR_APPEND_DYNAMIC(Object, Type) \
        ::Append<RPP_ACCESSOR_AS(DYNAMIC, #Type, Type)>
    #define RPP_ACCESSOR_APPEND_MEMBER(Object, Value) \
        ::Append<RPP_ACCESSOR_AS(MEMBER, #Value, Object, Value)>
    // append an accessor wraps a base type to a TypeList
    #define RPP_ACCESSOR_APPEND_BASE(Derived, Base) \
        RPP_ACCESSOR_APPEND_DYNAMIC(Derived, Base)
    // append accessors of a base type (fetched from AccessorObjectWrap) to a TypeList
    #define RPP_ACCESSOR_APPEND_BASE_INLINE(Derived, Base) \
        ::AppendList<AccessorInfer<Base &>::List>

    // build a TypeList of accessors
    #define RPP_MEMBER_IMPL(Member, Mode, ...) \
        , RPP_MEMBER_LIST), RPP_ACCESSOR_APPEND_##Mode, Member
    #define RPP_MEMBER_LIST_BEGIN(Object, Next) \
        Next##_1(Object
    #define RPP_MEMBER_LIST_1(Object, Mode, Member, Next) \
        Mode(Object, Member) Next##_2(Object
    #define RPP_MEMBER_LIST_2(Object, Mode, Member, Next) \
        Mode(Object, Member) Next##_1(Object
    #define RPP_MEMBER_LIST_END_1(...) /* nothing */
    #define RPP_MEMBER_LIST_END_2(...) /* nothing */

// helper macros end

// define a member in RPP_TYPE_OBJECT
#define RPP_MEMBER(...) \
    RPP_MEMBER_IMPL(__VA_ARGS__, MEMBER, )
#ifndef __
    #define __ RPP_MEMBER
#endif

// set the default accessor to AccessorObject and bind members
#define RPP_TYPE_OBJECT(Members, Object) \
    template <> \
    struct AccessorInfer<Object &>: public rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_MEMBER_LIST_BEGIN( \
            Object \
            Members \
            , RPP_MEMBER_LIST_END) \
        ) \
    > {};

// set the default accessor to AccessorDynamic and bind the (abstract) member
#define RPP_TYPE_DYNAMIC(Member, Object) \
    template <> \
    struct AccessorInfer<Object &>: public rpp::AccessorDynamicWrap< \
        RPP_ACCESSOR_AS(DYNAMIC, "member", Member) \
    > {};

// RPP_TYPE_OBJECT with template arguments
// __VA_ARGS__ is the type of object
#define RPP_TYPE_OBJECT_GENERIC(Members, ...) \
    /* before the macro: template <???> */ \
    struct AccessorInfer<rpp::RefCast<__VA_ARGS__>>: public rpp::AccessorObjectWrap< \
        rpp::TypeList<> \
        RPP_MEMBER_LIST_BEGIN( \
            RPP_PASS(__VA_ARGS__) \
            Members \
            , RPP_MEMBER_LIST_END) \
        ) \
    > {};

// RPP_TYPE_DYNAMIC with template arguments
// __VA_ARGS__ is the type of object
#define RPP_TYPE_DYNAMIC_GENERIC(Member, ...) \
    /* before the macro: template <???> */ \
    struct AccessorInfer<rpp::RefCast<__VA_ARGS__>>: public rpp::AccessorDynamicWrap< \
        RPP_ACCESSOR_AS(DYNAMIC, "member", Member) \
    > {};

}
