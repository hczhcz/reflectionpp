#include "common.hpp"

namespace rpp {

struct Visitor4: public VisitorBase<> {
    Visitor4() = default;
    Visitor4(Visitor4 &&) = delete;
    Visitor4(const Visitor4 &) = delete;

    void visit(int &value) {
        std::cerr << "int(" << value << ") ";
    }

    void visit(char &value) {
        std::cerr << "char(" << value << ") ";
        value += 1;
    }

    void visit(bool &value) {
        std::cerr << "bool(" << value << ") ";
    }

    void visit(float &value) {
        std::cerr << "float(" << value << ") ";
    }

    template <class T>
    void visit(const HolderType<T> &) {
        std::cerr << "type(" << typeid(T).name() << ") ";
    }

    template <class... Args>
    void operator()(AccessorSimple<Args...> &accessor) {
        visit(accessor());
    }

    template <class... Args>
    void operator()(AccessorObject<Args...> &accessor) {
        std::cerr << "object( ";

        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            accessor.doMemberVisit(*this, i);
        }

        std::cerr << ") ";
    }
};

struct Visitor5: public VisitorBase<const char *> {
    template <class T>
    const char *visit(T &value) {
        return typeid(value).name();
    }

    template <class Accessor>
    const char *operator()(Accessor &accessor) {
        return visit(accessor());
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

char value3{'C'};

}

namespace rpp_another_namespace {

using Accessor1 = rpp::Accessor1;
using Accessor2 = RPP_ACCESSOR_GET_AS(LOCAL, "value2", char);
using Accessor3 = RPP_ACCESSOR_GET_AS(REF, "value3", rpp::value3);
using Accessor4 = RPP_ACCESSOR_GET_AS(DYNAMIC, "value4", char);

using Accessor5m1 = RPP_ACCESSOR_GET_AS(MEMBER, "member1", TestStruct, member1);
using Accessor5m2 = RPP_ACCESSOR_GET_AS(MEMBER, "member2", TestStruct, member2);

using Accessor5 = RPP_ACCESSOR_GET_AS(LOCAL, "value5", TestStruct);
using Accessor6 = Accessor5::Meta;

using Accessor7m3 = RPP_ACCESSOR_GET_AS(MEMBER, "member3", TestStruct2, member3);
using Accessor7m4 = RPP_ACCESSOR_GET_AS(REF, "member4", TestStruct2::member4);
using Accessor7m5 = RPP_ACCESSOR_GET_AS(MEMBER, "member5", TestStruct2, member5);

using Accessor7 = RPP_ACCESSOR_GET_AS(LOCAL, "value7", TestStruct2);

static_assert(
    std::is_same<
        Accessor3,
        rpp::AccessorSimple<
            rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value3")>,
            rpp::HolderRef<char, rpp::value3>
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
        Accessor5m2,
        rpp::AccessorSimple<
            rpp::HolderConst<const char (&)[], RPP_STATIC_STR("member2")>,
            rpp::HolderMember<TestStruct, float, &TestStruct::member2>
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
        Accessor6,
        rpp::AccessorObject<
            rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value5")>,
            rpp::HolderType<TestStruct &>,
            rpp::TypeList<Accessor5m1::Meta, Accessor5m2::Meta>
        >
    >(), ""
);

static_assert(
    std::is_same<
        Accessor7,
        rpp::AccessorObject<
            rpp::HolderConst<const char (&)[], RPP_STATIC_STR("value7")>,
            rpp::HolderLocal<TestStruct2>,
            rpp::TypeList<
                rpp::AccessorObject<
                    rpp::HolderConst<const char (&)[], RPP_STATIC_STR("TestStruct")>,
                    rpp::HolderDynamic<TestStruct>,
                    rpp::TypeList<Accessor5m1, Accessor5m2>
                >,
                Accessor7m3, Accessor7m4, Accessor7m5
            >
        >
    >(), ""
);

static const int test_meta = []() {
    std::cerr << "======== test: " << __FILE__ << " ========" << std::endl;

    rpp::MetaImpl<Accessor1, rpp::VisitorAll3> meta1{0};
    rpp::MetaImpl<Accessor2, rpp::VisitorAll3> meta2{'A'};
    rpp::MetaImpl<Accessor3, rpp::VisitorAll3> meta3;
    rpp::MetaImpl<Accessor4, rpp::VisitorAll3> meta4{rpp::value3};
    rpp::MetaImpl<Accessor5, rpp::VisitorAll3> meta5{TestStruct{1, 1.5}};
    rpp::MetaImpl<Accessor6, rpp::VisitorAll3> meta6;
    rpp::MetaImpl<Accessor7, rpp::VisitorAll3> meta7{TestStruct2{1, '3', 5}};

    std::vector<rpp::MetaBase<rpp::VisitorAll3> *> metalist{
        &meta1, &meta2, &meta3, &meta4, &meta5, &meta6, &meta7
    };

    rpp::Visitor4 v4;
    rpp::Visitor5 v5;

    for (rpp::MetaBase<rpp::VisitorAll3> *meta: metalist) {
        std::cerr << meta->getName() << "\t";
        // std::cerr << meta->getTypeInfo().name() << ", ";
        // std::cerr << meta->getPointer() << " - ";
        meta->doVisit(v4);
        std::cerr << std::endl;
        std::cerr << "\ttype " << meta->doVisit(v5);
        std::cerr << std::endl;
    }

    std::cerr << "sizeof(Accessor):";
    std::cerr << " " << sizeof(Accessor1);
    std::cerr << " " << sizeof(Accessor2);
    std::cerr << " " << sizeof(Accessor3);
    std::cerr << " " << sizeof(Accessor4);
    std::cerr << " " << sizeof(Accessor5);
    std::cerr << " " << sizeof(Accessor6);
    std::cerr << " " << sizeof(Accessor7);
    std::cerr << std::endl;

    std::cerr << "sizeof(meta):";
    std::cerr << " " << sizeof(meta1);
    std::cerr << " " << sizeof(meta2);
    std::cerr << " " << sizeof(meta3);
    std::cerr << " " << sizeof(meta4);
    std::cerr << " " << sizeof(meta5);
    std::cerr << " " << sizeof(meta6);
    std::cerr << " " << sizeof(meta7);
    std::cerr << std::endl;

    return 0;
}();

}
