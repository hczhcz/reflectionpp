#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>
#include "../reflection++/visitor_chain.hpp"
#include "../reflection++/accessor_infer.hpp"
#include "../reflection++/meta.hpp"

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

    RPP_VISITOR_REG(Visitor4)
    RPP_VISITOR_REG(Visitor5)
    RPP_VISITOR_COLLECT(VisitorAll3)

    using Accessor1 = AccessorSimple<
        RPP_HOLDER_STR("value1"),
        RPP_HOLDER_LOCAL(int)
    >;

    static_assert(std::is_same<
        Accessor1,
        AccessorSimple<
            HolderConst<decltype(RPP_STATIC_STR("value1")), RPP_STATIC_STR("value1")>,
            HolderLocal<int>
        >
    >(), "");

    char accessor_value{'C'};

}

namespace rpp_another_namespace {

    RPP_ACCESSOR_INFER_INIT()

    using Accessor1 = rpp::Accessor1;

    using Accessor2 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("value2"),
        RPP_HOLDER_LOCAL(char)
    );

    using Accessor3 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("value3"),
        RPP_HOLDER_REF(rpp::accessor_value)
    );

    using Accessor4 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("value4"),
        RPP_HOLDER_DYNAMIC(char)
    );

    struct TestStruct {
        int member1;
        float member2;
    };

    using Accessor5m1 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("member1"),
        RPP_HOLDER_MEMBER(TestStruct, member1)
    );

    using Accessor5m2 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("member2"),
        RPP_HOLDER_MEMBER(TestStruct, member2)
    );

    using Accessor5old = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("value5"),
        rpp::HolderLocal<TestStruct>
    );

    RPP_ACCESSOR_INFER_BIND(TestStruct, Accessor5m1, Accessor5m2)

    using Accessor5 = RPP_ACCESSOR_INFER_GET(
        RPP_HOLDER_STR("value5"),
        rpp::HolderLocal<TestStruct>
    );

    using Accessor6 = Accessor5::Meta;

    static_assert(
        std::is_same<
            Accessor3,
            rpp::AccessorSimple<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value3")>,
                rpp::HolderRef<char, rpp::accessor_value>
            >
        >(), ""
    );

    static_assert(
        std::is_same<
            Accessor5old,
            rpp::AccessorSimple<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value5")>,
                rpp::HolderLocal<TestStruct>
            >
        >(), ""
    );

    static_assert(
        std::is_same<
            Accessor5m1,
            rpp::AccessorSimple<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("member1")>,
                rpp::HolderMember<TestStruct, int, &TestStruct::member1>
            >
        >(), ""
    );

    static_assert(
        std::is_same<
            Accessor5m1::Meta,
            rpp::AccessorSimple<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("member1")>,
                rpp::HolderType<int &>
            >
        >(), ""
    );

    static_assert(
        std::is_same<
            Accessor5,
            rpp::AccessorObject<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value5")>,
                rpp::HolderLocal<TestStruct>,
                rpp::TypeList<Accessor5m1, Accessor5m2>
            >
        >(), ""
    );

    static_assert(
        std::is_same<
            Accessor5::Meta,
            rpp::AccessorObject<
                rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value5")>,
                rpp::HolderType<TestStruct &>,
                rpp::TypeList<Accessor5m1::Meta, Accessor5m2::Meta>
            >
        >(), ""
    );

    static const int test1 = []() {
        rpp::MetaImpl<rpp::VisitorAll3, Accessor1> meta1{0};
        rpp::MetaImpl<rpp::VisitorAll3, Accessor2> meta2{'A'};
        rpp::MetaImpl<rpp::VisitorAll3, Accessor3> meta3;
        rpp::MetaImpl<rpp::VisitorAll3, Accessor4> meta4{rpp::accessor_value};
        rpp::MetaImpl<rpp::VisitorAll3, Accessor5> meta5{TestStruct{1, 1.5}};
        rpp::MetaImpl<rpp::VisitorAll3, Accessor6> meta6;

        std::vector<rpp::MetaBase<rpp::VisitorAll3> *> metalist{
            &meta1, &meta2, &meta3, &meta4, &meta5, &meta6
        };

        rpp::Visitor4 v4;
        rpp::Visitor5 v5;

        for (rpp::MetaBase<rpp::VisitorAll3> *meta: metalist) {
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
