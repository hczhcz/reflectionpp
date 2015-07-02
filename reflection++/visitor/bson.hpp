#pragma once

#include <functional>
#include <string>
#include <memory>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

// forward declaration
template <class Doc>
struct VisitorBSONRootDocBase;

// render a BSON (MongoDB data) object
template <class Base = VisitorBSONRootDocBase<bsoncxx::builder::basic::document>>
struct VisitorBSON: public Base {
public:
    using Base::Base;

    // pointer

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::shared_ptr<Args...> &value) {
        this->visitPtr(this, accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::weak_ptr<Args...> &value) {
        this->visitPtr(this, accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unique_ptr<Args...> &value) {
        this->visitPtr(this, accessor, value);
    }

    template <class Accessor, class T>
    void into(Accessor &accessor, T *&value) {
        this->visitPtr(this, accessor, value);
    }

    // array

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, T (&value)[size]) {
        this->visitArr(accessor, value);
    }

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, std::array<T, size> &value) {
        this->visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::deque<Args...> &value) {
        this->visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::forward_list<Args...> &value) {
        this->visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::list<Args...> &value) {
        this->visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::vector<Args...> &value) {
        this->visitArr(accessor, value);
    }

    // map

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::map<Args...> &value) {
        this->visitMap(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unordered_map<Args...> &value) {
        this->visitMap(accessor, value);
    }

    // handle different types of accessors

    template <class... Args>
    void operator()(AccessorSimple<Args...> &accessor) {
        this->visitVal(accessor());
    }

    template <class... Args>
    void operator()(AccessorObject<Args...> &accessor) {
        this->visitObj(accessor);
    }

    template <class... Args>
    void operator()(AccessorDynamic<Args...> &accessor) {
        into(accessor, accessor());
    }
};

// hold a document entry
template <class Doc>
struct VisitorBSONAddDocBase: public VisitorBase<void> {
private:
    Doc &doc;
    std::string name;

protected:
    template <class Member>
    void add(Member &&member) {
        doc.append(
            bsoncxx::builder::basic::kvp(
                name, static_cast<Member &&>(member)
            )
        );
    }

    template <class Member>
    void add(const Member &member) {
        doc.append(
            bsoncxx::builder::basic::kvp(
                name, member
            )
        );
    }

public:
    VisitorBSONAddDocBase(Doc &_doc, const std::string &_name):
        doc{_doc}, name{_name} {}
};

// hold an array
template <class Arr>
struct VisitorBSONAddArrBase: public VisitorBase<void> {
private:
    Arr &arr;

protected:
    template <class Member>
    void add(Member &&member) {
        arr.append(
            static_cast<Member &&>(member)
        );
    }

    template <class Member>
    void add(const Member &member) {
        arr.append(
            member
        );
    }

public:
    VisitorBSONAddArrBase(Arr &_arr):
        arr{_arr} {}
};

// build a BSON object as an item in a document or an array
template <class Base>
struct VisitorBSONItemBase: public Base {
protected:
    template <class T>
    void visitVal(const T &value) {
        this->add(value);
    }

    void visitVal(const uint32_t &value) {
        this->add(static_cast<int32_t>(value));
    }

    void visitVal(const uint64_t &value) {
        this->add(static_cast<int64_t>(value));
    }

    void visitVal(const char &value) {
        this->add(std::string{} + value);
    }

    // void visitVal(const wchar_t &value) {
    //     this->add(std::string{} + value);
    // }

    template <class Self, class Accessor, class T>
    void visitPtr(Self *self, Accessor &accessor, T &value) {
        if (value) {
            accessor.doMemberVisit(*self, *value);
        } else {
            // notice: b_null is discouraged
            this->visitVal(bsoncxx::types::b_null{});
        }
    }

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) {
        using bsoncxx::builder::basic::sub_array;

        this->add([&](sub_array arr) {
            auto begin = std::begin(value);
            auto end = std::end(value);
            for (auto i = begin; i != end; ++i) {
                VisitorBSON<
                    VisitorBSONItemBase<
                        VisitorBSONAddArrBase<sub_array>
                    >
                > child{arr};

                accessor.doMemberVisit(child, *i);
            }
        });
    }

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) {
        using bsoncxx::builder::basic::sub_document;

        this->add([&](sub_document doc) {
            auto begin = std::begin(value);
            auto end = std::end(value);
            for (auto i = begin; i != end; ++i) {
                VisitorBSON<
                    VisitorBSONItemBase<
                        VisitorBSONAddDocBase<sub_document>
                    >
                > child{doc, i->first};

                accessor.doMemberVisit(child, i->second);
            }
        });
    }

    template <class... Args>
    void visitObj(AccessorObject<Args...> &accessor) {
        using bsoncxx::builder::basic::sub_document;

        this->add([&](sub_document doc) {
            for (rpp_size_t i = 0; i < accessor.size(); ++i) {
                VisitorBSON<
                    VisitorBSONItemBase<
                        VisitorBSONAddDocBase<sub_document>
                    >
                > child{doc, accessor.getMemberName(i)};

                accessor.doMemberVisit(child, i);
            }
        });
    }

public:
    using Base::Base;
};

// build a BSON object as a root (type: document)
template <class Doc = bsoncxx::builder::basic::document>
struct VisitorBSONRootDocBase: public VisitorBase<void>, public Doc {
protected:
    template <class T>
    void visitVal(const T &value) = delete;

    template <class Self, class Accessor, class T>
    void visitPtr(Self *self, Accessor &accessor, T &value) = delete;

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) = delete;

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) {
        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            VisitorBSON<
                VisitorBSONItemBase<
                    VisitorBSONAddDocBase<Doc>
                >
            > child{*this, i->first};

            accessor.doMemberVisit(child, i->second);
        }
    }

    template <class... Args>
    void visitObj(AccessorObject<Args...> &accessor) {
        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            VisitorBSON<
                VisitorBSONItemBase<
                    VisitorBSONAddDocBase<Doc>
                >
            > child{*this, accessor.getMemberName(i)};

            accessor.doMemberVisit(child, i);
        }
    }
};

// build a BSON object as root (type: array)
template <class Arr = bsoncxx::builder::basic::array>
struct VisitorBSONRootArrBase: public VisitorBase<void>, public Arr {
protected:
    template <class T>
    void visitVal(const T &value) = delete;

    template <class Self, class Accessor, class T>
    void visitPtr(Self *self, Accessor &accessor, T &value) = delete;

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) {
        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            VisitorBSON<
                VisitorBSONItemBase<
                    VisitorBSONAddArrBase<Arr>
                >
            > child{*this};

            accessor.doMemberVisit(child, *i);
        }
    }

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) = delete;

    template <class... Args>
    void visitObj(AccessorObject<Args...> &accessor) = delete;
};

}
