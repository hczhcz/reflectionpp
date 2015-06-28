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

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

template <class Arr>
struct BSONArrAdder {
    Arr &arr;

    BSONArrAdder(Arr &_arr):
        arr{_arr} {}

    template <class Member>
    void operator()(const Member &member) {
        arr.append(member);
    }
};

template <class Doc>
struct BSONDocAdder {
    Doc &doc;
    std::string name;

    BSONDocAdder(Doc &_doc, const std::string &_name):
        doc{_doc}, name{_name} {}

    template <class Member>
    void operator()(const Member &member) {
        doc.append(kvp(name, member));
    }
};

// render a BSON (MongoDB document) object
template <class Adder>
struct VisitorBSON: public VisitorBase<void> {
private:
    Adder &adder;

    template <class Accessor, class T>
    void visitPtr(Accessor &accessor, T &value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        } else {
            // TODO
        }
    }

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) {
        using namespace std::placeholders;
        using namespace bsoncxx::builder::basic;

        adder([&](sub_array arr) {
            auto begin = std::begin(value);
            auto end = std::end(value);
            for (auto i = begin; i != end; ++i) {
                BSONArrAdder<sub_array> child_adder{
                    arr
                };

                VisitorBSON<decltype(child_adder)> child{child_adder};

                accessor.doMemberVisit(child, *i);
            }
        });
    }

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) {
        using namespace std::placeholders;
        using namespace bsoncxx::builder::basic;

        adder([&](sub_document doc) {
            auto begin = std::begin(value);
            auto end = std::end(value);
            for (auto i = begin; i != end; ++i) {
                BSONDocAdder<sub_document> child_adder{
                    i->first, doc
                };

                VisitorBSON<decltype(child_adder)> child{child_adder};

                accessor.doMemberVisit(child, i->second);
            }
        });
    }

public:
    VisitorBSON(Adder &_adder): adder{_adder} {}

    template <class T>
    void visit(const T &value) {
        adder(value);
    }

    // pointer

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::shared_ptr<Args...> &value) {
        visitPtr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::weak_ptr<Args...> &value) {
        visitPtr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unique_ptr<Args...> &value) {
        visitPtr(accessor, value);
    }

    template <class Accessor, class T>
    void into(Accessor &accessor, T *&value) {
        visitPtr(accessor, value);
    }

    // array

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, T (&value)[size]) {
        visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::array<Args...> &value) {
        visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::deque<Args...> &value) {
        visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::forward_list<Args...> &value) {
        visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::list<Args...> &value) {
        visitArr(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::vector<Args...> &value) {
        visitArr(accessor, value);
    }

    // map

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::map<Args...> &value) {
        visitMap(accessor, value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unordered_map<Args...> &value) {
        visitMap(accessor, value);
    }

    // handle different types of accessors

    template <class... Args>
    void operator()(AccessorSimple<Args...> &accessor) {
        visit(accessor());
    }

    template <class... Args>
    void operator()(AccessorObject<Args...> &accessor) {
        using namespace std::placeholders;
        using namespace bsoncxx::builder::basic;

        adder([&](sub_document doc) {
            for (rpp_size_t i = 0; i < accessor.size(); ++i) {
                BSONDocAdder<sub_document> child_adder{
                    accessor.getMemberName(i), doc
                };

                VisitorBSON<decltype(child_adder)> child{child_adder};

                accessor.doMemberVisit(child, i);
            }
        });
    }

    template <class... Args>
    void operator()(AccessorDynamic<Args...> &accessor) {
        into(accessor, accessor());
    }
};

}
