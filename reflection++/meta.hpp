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
template <class Name, class Accessor, class Visitors, class Base = MetaBase<Visitors>>
struct MetaImpl;

template <class Name, class Accessor, class Base>
struct MetaImpl<
    Name, Accessor, TypeList<>, Base
>: public Base, protected Name, protected Accessor {
    using Accessor::Accessor;
    using Base::doVisit;

    virtual const char *getName() override {
        return (*static_cast<Name *>(this))();
    }
};

template <class Name, class Accessor, class Visitor, class... Args, class Base>
struct MetaImpl<
    Name, Accessor, TypeList<Visitor, Args...>, Base
>: public MetaImpl<Name, Accessor, TypeList<Args...>, Base> {
    using MetaImpl<Name, Accessor, TypeList<Args...>, Base>::MetaImpl;
    using MetaImpl<Name, Accessor, TypeList<Args...>, Base>::doVisit;

    virtual typename Visitor::ReturnType doVisit(Visitor &visitor) override {
        return visitor(*static_cast<Accessor *>(this));
    }
};

// generate a meta info type
// wrappers of RPP_ACCESSOR_AS
#define RPP_META_TYPE(Type, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(#Type), \
        RPP_ACCESSOR_AS(TYPE, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_CONST(Value, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(#Value), \
        RPP_ACCESSOR_AS(CONST, Value), \
        __VA_ARGS__ \
    >
#define RPP_META_LOCAL(Name, Type, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(Name), \
        RPP_ACCESSOR_AS(LOCAL, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_REF(Value, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(#Value), \
        RPP_ACCESSOR_AS(REF, Value), \
        __VA_ARGS__ \
    >
#define RPP_META_DYNAMIC(Name, Type, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(Name), \
        RPP_ACCESSOR_AS(DYNAMIC, Type), \
        __VA_ARGS__ \
    >
#define RPP_META_MEMBER(Object, Value, ...) \
    rpp::MetaImpl< \
        RPP_HOLDER_STR(#Value), \
        RPP_ACCESSOR_AS(MEMBER, Object, Value), \
        __VA_ARGS__ \
    >

}
