#pragma once

#include <sstream>
#include <string>
#include <memory>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

// read value from a tree of strings
template <class Node, class Stream = std::istringstream, bool strict = true>
struct VisitorIStrTree: public VisitorBase<void> {
private:
    Node &node;

public:
    VisitorIStrTree(Node &_node): node{_node} {}

    template <class T>
    void visit(T &value) {
        Stream buf{node};

        buf >> value;

        if (!buf.eof()) {
            throw Exception{};
        }
    }

    template <class... Args>
    void visit(std::basic_string<Args...> &value) { // TODO
        value = node;
    }

    // pointer

    template <class Accessor, class T>
    void into(Accessor &accessor, std::shared_ptr<T> &value) {
        if (!value) {
            value = new T{}; // TODO: argument T?
        }

        accessor.doMemberVisit(*this, *value);
    }

    template <class Accessor, class T>
    void into(Accessor &accessor, std::weak_ptr<T> &value) {
        if (!value) {
            value = new T{}; // TODO: argument T?
        }

        accessor.doMemberVisit(*this, *value);
    }

    template <class Accessor, class T, class... Args>
    void into(Accessor &accessor, std::unique_ptr<T, Args...> &value) {
        if (!value) {
            value = std::unique_ptr<T, Args...>{new T{}}; // TODO: argument T?
        }

        accessor.doMemberVisit(*this, *value);
    }

    template <class Accessor, class T>
    void into(Accessor &accessor, T *&value) {
        if (!value) {
            value = new T{};
        }

        accessor.doMemberVisit(*this, *value);
    }

    // array

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, T (&value)[size]) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, std::array<T, size> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::deque<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::forward_list<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::list<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::vector<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    // map

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::map<Args...> &value) {
        // this->visitMap(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unordered_map<Args...> &value) {
        // this->visitMap(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    // handle different types of accessors

    template <class... Args>
    void operator()(AccessorSimple<Args...> &accessor) {
        visit(accessor());
    }

    template <class... Args>
    void operator()(AccessorObject<Args...> &accessor) {
        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            VisitorIStrTree<
                decltype(node[""]), Stream
            > child{
                strict ? node.at(accessor.getMemberName(i))
                       : node[accessor.getMemberName(i)]
            };

            accessor.doMemberVisit(child, i);
        }
    }

    template <class... Args>
    void operator()(AccessorDynamic<Args...> &accessor) {
        into(accessor, accessor());
    }
};

// build a tree of strings
template <class Node, class Stream = std::ostringstream>
struct VisitorOStrTree: public VisitorBase<void> {
private:
    Node &node;

public:
    VisitorOStrTree(Node &_node): node{_node} {}

    template <class T>
    void visit(T &value) {
        Stream buf;

        buf << value;
        node = buf.str();
    }

    template <class... Args>
    void visit(const std::basic_string<Args...> &value) { // TODO
        node = value;
    }

    // pointer

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::shared_ptr<Args...> &value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        }
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::weak_ptr<Args...> &value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        }
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unique_ptr<Args...> &value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        }
    }

    template <class Accessor, class T>
    void into(Accessor &accessor, T *&value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        }
    }

    // array

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, T (&value)[size]) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, std::array<T, size> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::deque<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::forward_list<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::list<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::vector<Args...> &value) {
        // this->visitArr(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    // map

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::map<Args...> &value) {
        // this->visitMap(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    template <class Accessor, class... Args>
    void into(Accessor &accessor, std::unordered_map<Args...> &value) {
        // this->visitMap(accessor, value); // TODO
        static_cast<void>(accessor);
        static_cast<void>(value);
    }

    // handle different types of accessors

    template <class... Args>
    void operator()(AccessorSimple<Args...> &accessor) {
        visit(accessor());
    }

    template <class... Args>
    void operator()(AccessorObject<Args...> &accessor) {
        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            VisitorOStrTree<
                decltype(node[""]), Stream
            > child{
                node[accessor.getMemberName(i)]
            };

            accessor.doMemberVisit(child, i);
        }
    }

    template <class... Args>
    void operator()(AccessorDynamic<Args...> &accessor) {
        into(accessor, accessor());
    }
};

}
