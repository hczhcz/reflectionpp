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
template <class Accessor, class Visitors, class Base = MetaBase<Visitors>>
struct MetaImpl;

template <class Accessor, class Base>
struct MetaImpl<
    Accessor, TypeList<>, Base
>: public Base, protected Accessor {
    using Accessor::Accessor;
    using Base::doVisit;

    virtual const char *getName() override {
        return Accessor::getRealName();
    }
};

template <class Accessor, class Visitor, class... Args, class Base>
struct MetaImpl<
    Accessor, TypeList<Visitor, Args...>, Base
>: public MetaImpl<Accessor, TypeList<Args...>, Base> {
    using MetaImpl<Accessor, TypeList<Args...>, Base>::MetaImpl;
    using MetaImpl<Accessor, TypeList<Args...>, Base>::doVisit;

    virtual typename Visitor::ReturnType doVisit(Visitor &visitor) override {
        return visitor(*static_cast<Accessor *>(this));
    }
};

// generate a meta info type
// wrappers of RPP_ACCESSOR_AS
#define RPP_META_TYPE(Type, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(TYPE, #Type, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_CONST(Value, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(CONST, #Value, Value), \
        __VA_ARGS__ \
    >
#define RPP_META_LOCAL(Name, Type, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(LOCAL, Name, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_REF(Value, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(REF, #Value, Value), \
        __VA_ARGS__ \
    >
#define RPP_META_DYNAMIC(Name, Type, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(DYNAMIC, Name, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_MEMBER(Object, Value, ...) \
    rpp::MetaImpl< \
        RPP_ACCESSOR_AS(MEMBER, #Value, Object, Value), \
        __VA_ARGS__ \
    >

}
