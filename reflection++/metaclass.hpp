#pragma once

#include "visitor.hpp"

namespace rpp {

// the base class of meta information containers
template <class Visitors>
struct MetaBase;

template <>
struct MetaBase<VisitorList<>> {
    virtual const char *getName();
};

template <class Visitor, class... Args>
struct MetaBase<VisitorList<Visitor, Args...>>: public MetaBase<VisitorList<Args...>> {
    using Visitors = VisitorList<Visitor, Args...>;

    virtual typename Visitor::ReturnValue visit(Visitor &visitor) = 0;
};

// TODO

// #define META_MEMBER(VALUE) \
//     template <class Base, class... Args> \
//     struct MetaMemberBuilder_##VALUE; \
//     \
//     template <class Base> \
//     struct MetaMemberBuilder_##VALUE<Base>: public Base { \
//         const decltype(VALUE) &value = VALUE; \
//     \
//         inline MetaMemberBuilder_##VALUE(): Base(#VALUE) {} \
//     }; \
//     \
//     template <class Base, class Visitor, class... Args> \
//     struct MetaMemberBuilder_##VALUE<Base, Visitor, Args...>: public MetaMemberBuilder_##VALUE<Base, Args...> { \
//         virtual typename Visitor::ReturnValue visit(Visitor &visitor) { \
//             return visitor.visit(VALUE); \
//         } \
//     }; \
//     \
//     template <class... Args> \
//     using MetaMember_##VALUE = MetaMemberBuilder_##VALUE<rpp::MetaMemberBase<Args...>, Args...>;

// class Example {
// public:
//     int a;
//     bool b;
// };

// class MetaExample: public Meta<Example> {
// public:
//     META_MEMBER(a)
//     META_MEMBER(b)
// };

}
