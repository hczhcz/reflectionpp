#pragma once

#include "type_list.hpp"
#include "static_str.hpp"

namespace rpp {

// the base class of meta information containers
template <class Visitors>
struct MetaBase;

template <>
struct MetaBase<
    TypeList<>
> {
    virtual const char *getName() = 0;

    // TODO: add common visitors
    // virtual const std::type_info &getTypeInfo() = 0;
    // virtual void *getPointer() = 0;

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
        return Accessor::doRealVisit(visitor);
    }
};

}

// ======== Usage example ========

#ifdef RPP_DEBUG

    #include <typeinfo>
    #include <type_traits>
    #include <iostream>
    #include <string>
    #include <vector>
    #include "visitor_chain.hpp"
    #include "holder.hpp"
    #include "accessor.hpp"

    namespace rpp {

        struct Visitor4: public VisitorBase<> {
            void visit(int &value) {
                std::cerr << "int, " << value;
            }

            void visit(char &value) {
                std::cerr << "char, " << value;
                value += 1;
            }

            void visit(bool &value) {
                std::cerr << "bool, " << value;
            }

            void visit(float &value) {
                std::cerr << "float, " << value;
            }

            template <class T>
            void visit(const HolderType<T> &) {
                std::cerr << "type, " << typeid(T).name();
            }

            template <class Accessor>
            void into(Accessor &accessor) {
                std::cerr << "object";

                for (rpp_size_t i = 0; i < accessor.size(); ++i) {
                    std::cerr << " << ";
                    accessor.doMemberVisit(*this, i);
                }
            }
        };

        struct Visitor5: public VisitorBase<const char *> {
            template <class T>
            const char *visit(T &value) {
                return typeid(value).name();
            }

            template <class Accessor>
            const char *into(Accessor &accessor) {
                return accessor.doObjectVisit(*this);
            }
        };

        using Accessor1 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("value1")>,
            HolderLocal<int>
        >;

        using Accessor2 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("value2")>,
            HolderLocal<char>
        >;

        namespace {
            char accessor_value = 'C';
        }

        using Accessor3 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("value3")>,
            HolderRef<char, accessor_value>
        >;

        using Accessor4 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("value4")>,
            HolderDynamic<char>
        >;

        struct TestStruct {
            int member1;
            float member2;
        };

        using Accessor5m1 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("member1")>,
            HolderMember<TestStruct, int, &TestStruct::member1>
        >;

        using Accessor5m2 = AccessorSimple<
            HolderConst<const char *, RPP_STATIC_STR("member2")>,
            HolderMember<TestStruct, float, &TestStruct::member2>
        >;

        using Accessor5 = AccessorObject<
            HolderConst<const char *, RPP_STATIC_STR("value5")>,
            HolderLocal<TestStruct>,
            TypeList<Accessor5m1, Accessor5m2>
        >;

        static_assert(
            std::is_same<
                Accessor5m1::Meta,
                AccessorSimple<
                    HolderConst<const char *, RPP_STATIC_STR("member1")>,
                    HolderType<int &>
                >
            >(), ""
        );

        static_assert(
            std::is_same<
                Accessor5::Meta,
                AccessorObject<
                    HolderConst<const char *, RPP_STATIC_STR("value5")>,
                    HolderType<TestStruct &>,
                    TypeList<Accessor5m1::Meta, Accessor5m2::Meta>
                >
            >(), ""
        );

        using Accessor6 = Accessor5::Meta;

        RPP_VISITOR_REG(Visitor4)
        RPP_VISITOR_REG(Visitor5)
        RPP_VISITOR_COLLECT(VisitorAll3)

        static const int test1 = []() {
            MetaImpl<VisitorAll3, Accessor1> meta1{0};
            MetaImpl<VisitorAll3, Accessor2> meta2{'A'};
            MetaImpl<VisitorAll3, Accessor3> meta3;
            MetaImpl<VisitorAll3, Accessor4> meta4{accessor_value};
            MetaImpl<VisitorAll3, Accessor5> meta5{TestStruct{1, 1.5}};
            MetaImpl<VisitorAll3, Accessor6> meta6;

            std::vector<MetaBase<VisitorAll3> *> metalist{
                &meta1, &meta2, &meta3, &meta4, &meta5, &meta6
            };

            Visitor4 v4;
            Visitor5 v5;

            for (MetaBase<VisitorAll3> *meta: metalist) {
                std::cerr << meta->getName() << ": ";
                // std::cerr << meta->getTypeInfo().name() << ", ";
                // std::cerr << meta->getPointer() << " - ";
                meta->doVisit(v4);
                std::cerr << " - type " << meta->doVisit(v5);
                std::cerr << std::endl;
            }

            std::cerr << "sizeof(Accessor):";
            std::cerr << " " << sizeof(Accessor1);
            std::cerr << " " << sizeof(Accessor2);
            std::cerr << " " << sizeof(Accessor3);
            std::cerr << " " << sizeof(Accessor4);
            std::cerr << " " << sizeof(Accessor5);
            std::cerr << " " << sizeof(Accessor6);
            std::cerr << std::endl;

            std::cerr << "sizeof(meta):";
            std::cerr << " " << sizeof(meta1);
            std::cerr << " " << sizeof(meta2);
            std::cerr << " " << sizeof(meta3);
            std::cerr << " " << sizeof(meta4);
            std::cerr << " " << sizeof(meta5);
            std::cerr << " " << sizeof(meta6);
            std::cerr << std::endl;

            return 0;
        }();

    }

#endif
