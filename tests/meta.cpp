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
    // TODO, see visitor_chain.hpp
    // Visitor4(const Visitor4 &) = delete;
    // Visitor4(Visitor4 &&) = default;

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

RPP_ACCESSOR_INFER_INIT()

using Accessor1 = rpp::Accessor1;

using Accessor2 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value2"),
    RPP_HOLDER_LOCAL(char)
);

using Accessor3 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value3"),
    RPP_HOLDER_REF(rpp::value3)
);

using Accessor4 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value4"),
    RPP_HOLDER_DYNAMIC(char)
);

struct TestStruct {
    int member1;
    float member2;
};

using Accessor5m1 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("member1"),
    RPP_HOLDER_MEMBER(TestStruct, member1)
);

using Accessor5m2 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("member2"),
    RPP_HOLDER_MEMBER(TestStruct, member2)
);

using Accessor5old = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value5"),
    RPP_HOLDER_LOCAL(TestStruct)
);

RPP_TYPE_OBJECT(
    TestStruct,
    __(member1)
    __(member2, MEMBER) // the same as __(member2)
)

using Accessor5 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value5"),
    RPP_HOLDER_LOCAL(TestStruct)
);

using Accessor6 = Accessor5::Meta;

struct TestStruct2: public TestStruct {
    char member3;
    static int member4;
    TestStruct member5;

    TestStruct2(int a, char b, int c):
        TestStruct{a, a + 1.0f}, member3{b}, member5{c, c + 1.0f} {}
};
int TestStruct2::member4{4};

using Accessor7m3 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("member3"),
    RPP_HOLDER_MEMBER(TestStruct2, member3)
);

using Accessor7m4 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("member4"),
    RPP_HOLDER_REF(TestStruct2::member4)
);

using Accessor7m5 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("member5"),
    RPP_HOLDER_MEMBER(TestStruct2, member5)
);

RPP_TYPE_OBJECT(
    TestStruct2,
    __(TestStruct, BASE)
    __(member3)
    __(member4, REF)
    __(member5)
)

using Accessor7 = RPP_ACCESSOR_GET(
    RPP_HOLDER_STR("value7"),
    RPP_HOLDER_LOCAL(TestStruct2)
);

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

static const int test1 = []() {
    rpp::MetaImpl<rpp::VisitorAll3, Accessor1> meta1{0};
    rpp::MetaImpl<rpp::VisitorAll3, Accessor2> meta2{'A'};
    rpp::MetaImpl<rpp::VisitorAll3, Accessor3> meta3;
    rpp::MetaImpl<rpp::VisitorAll3, Accessor4> meta4{rpp::value3};
    rpp::MetaImpl<rpp::VisitorAll3, Accessor5> meta5{TestStruct{1, 1.5}};
    rpp::MetaImpl<rpp::VisitorAll3, Accessor6> meta6;
    rpp::MetaImpl<rpp::VisitorAll3, Accessor7> meta7{TestStruct2{1, '3', 5}};

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
