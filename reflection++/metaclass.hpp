#pragma once

#include "visitor.hpp"

namespace rpp {

// the base class of meta information containers
template <class Visitors>
struct MetaBase;

template <>
struct MetaBase<VisitorList<>> {
    virtual const char *getName() = 0;

    virtual void visit(void) {};
};

template <class Visitor, class... Args>
struct MetaBase<VisitorList<Visitor, Args...>>: public MetaBase<VisitorList<Args...>> {
    using MetaBase<VisitorList<Args...>>::visit;

    virtual typename Visitor::ReturnValue visit(Visitor &visitor) = 0;
};

// the implementation of MetaBase
template <class Visitors, const char Name[], class Accessor, class Base = MetaBase<Visitors>>
struct MetaImpl;

template <const char Name[], class Accessor, class Base>
struct MetaImpl<
    VisitorList<>, Name, Accessor, Base
>: public Base, Accessor {
    virtual const char *getName() {
        return Name;
    }
};

template <class Visitor, class... Args, const char Name[], class Accessor, class Base>
struct MetaImpl<
    VisitorList<Visitor, Args...>, Name, Accessor, Base
>: public MetaImpl<VisitorList<Args...>, Name, Accessor, Base> {
    using Base::visit;

    virtual typename Visitor::ReturnValue visit(Visitor &visitor) override {
        return visitor.visit(this->access());
    }
};

// ======== Usage example ========

#ifdef RPP_DEBUG

    #include <iostream>
    #include "visitor_list.hpp"

    struct Visitor4: public VisitorBase<> {
        void visit(int value) {
            std::cerr << "Visitor4: int, " << value << std::endl;
        }

        void visit(char value) {
            std::cerr << "Visitor4: char, " << value << std::endl;
        }

        void visit(bool value) {
            std::cerr << "Visitor4: bool, " << value << std::endl;
        }
    };

    const char value_name_1[] = "value1";
    struct Accessor1 {
        int value = 0;

        int &access() {
            return value;
        }
    };

    const char value_name_2[] = "value2";
    struct Accessor2 {
        char value = 'A';

        char &access() {
            return value;
        }
    };

    RPP_VISITOR_REG(Visitor4)
    RPP_VISITOR_COLLECT(VisitorAll3)

    MetaImpl<VisitorAll3, value_name_1, Accessor1> Meta1;
    MetaImpl<VisitorAll3, value_name_2, Accessor2> Meta2;

#endif

}
