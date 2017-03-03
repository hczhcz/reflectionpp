Reflection++
===

Yet another (?) header-only C++11 reflection library.

Usage
---

Add meta information to an object:

    struct TestStruct {
        int member1;
        float member2;
    };
    RPP_TYPE_OBJECT(
        __(member1)
        __(member2),
        TestStruct
    )

Add meta information to an STL container:

    template <class T, class... Args>
    RPP_TYPE_DYNAMIC_GENERIC(T, std::vector<T, Args...>)

Create a visitor:

    struct Visitor: public VisitorBase<> {
        void visit(int &value) {
            std::cerr << "int(" << value << ") ";
        }

        void visit(float &value) {
            std::cerr << "float(" << value << ") ";
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

Register the visitor and list all visitors:

    RPP_VISITOR_REG(Visitor)
    RPP_VISITOR_COLLECT(VisitorAll)

Add meta information to a variable:

    TestStruct s{2, 3.5};
    RPP_META_REF(s, VisitorAll) m;

Visit the variable:

    Visitor v;
    m.doVisit(v);

Build
---

Compile it like this:

    clang++ -std=c++11 your_code.cpp

If you want to use the BSON visitor, [bson-cxx](https://github.com/mongodb/mongo-cxx-driver) is required.
