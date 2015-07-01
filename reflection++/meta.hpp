#pragma once

#include "accessor_infer.hpp"

namespace rpp {

// the base class of meta information containers
template <class Visitors>
struct MetaBase;

template <>
struct MetaBase<
    TypeList<>
> {
    virtual const char *getName() = 0;

    // see: using MetaBase<TypeList<Args...>>::doVisit
    void doVisit() = delete;
};

template <class Visitor, class... Args>
struct MetaBase<
    TypeList<Visitor, Args...>
>: public MetaBase<TypeList<Args...>> {
    using MetaBase<TypeList<Args...>>::doVisit;

    virtual typename Visitor::ReturnType doVisit(Visitor &visitor) = 0;
};

// the implementation of MetaBase
// implemented vitrual functions
template <class Visitors, class Accessor, class Base = MetaBase<Visitors>>
struct MetaImpl;

template <class Accessor, class Base>
struct MetaImpl<
    TypeList<>, Accessor, Base
>: public Base, protected Accessor {
    using Accessor::Accessor;
    using Base::doVisit;

    virtual const char *getName() override {
        return Accessor::getRealName();
    }
};

template <class Visitor, class... Args, class Accessor, class Base>
struct MetaImpl<
    TypeList<Visitor, Args...>, Accessor, Base
>: public MetaImpl<TypeList<Args...>, Accessor, Base> {
    using MetaImpl<TypeList<Args...>, Accessor, Base>::MetaImpl;
    using MetaImpl<TypeList<Args...>, Accessor, Base>::doVisit;

    virtual typename Visitor::ReturnType doVisit(Visitor &visitor) override {
        return visitor(*static_cast<Accessor *>(this));
    }
};

// generate a meta info type
// wrappers of RPP_ACCESSOR_GET_AS
#define RPP_META_TYPE(Type, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(#Type, TYPE, Type) \
    >
#define RPP_META_CONST(Value, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(#Value, CONST, Value) \
    >
#define RPP_META_LOCAL(Name, Type, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(Name, LOCAL, Type) \
    >
#define RPP_META_REF(Value, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(#Value, REF, Value) \
    >
#define RPP_META_DYNAMIC(Name, Type, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(Name, DYNAMIC, Type) \
    >
#define RPP_META_MEMBER(Object, Value, ...) \
    rpp::MetaImpl< \
        __VA_ARGS__, \
        RPP_ACCESSOR_GET_AS(#Value, MEMBER, Object, Value) \
    >

}
