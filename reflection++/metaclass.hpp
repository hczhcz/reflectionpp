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

    #include <typeinfo>
    #include <iostream>
    #include "visitor_chain.hpp"

    struct Visitor4: public VisitorBase<> {
        void visit(int &value) {
            std::cerr << "Visitor4: int, " << value << std::endl;
        }

        void visit(char &value) {
            std::cerr << "Visitor4: char, " << value << std::endl;
            value = 'B';
        }

        void visit(bool &value) {
            std::cerr << "Visitor4: bool, " << value << std::endl;
        }
    };

    struct Visitor5: public VisitorBase<> {
        template <class T>
        void visit(T &value) {
            std::cerr << "Visitor5: " << typeid(T).name() << ", " << value << std::endl;
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
    RPP_VISITOR_REG(Visitor5)
    RPP_VISITOR_COLLECT(VisitorAll3)

    static const int test1 = []() {
        MetaImpl<VisitorAll3, value_name_1, Accessor1> meta1;
        MetaImpl<VisitorAll3, value_name_2, Accessor2> meta2;

        Visitor4 v4;
        Visitor5 v5;

        meta1.visit(v4);
        meta2.visit(v4);
        meta1.visit(v5);
        meta2.visit(v5);

        return 0;
    }();

#endif

}
