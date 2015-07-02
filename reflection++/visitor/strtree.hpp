#pragma once

#include <sstream>

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

    template <class Accessor, class T>
    void into(Accessor &, T &value) {
        // TODO: support STL classes
        Stream buf{node};

        buf >> value;
    }

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

    template <class Accessor, class T>
    void into(Accessor &, T &value) {
        // TODO: support STL classes
        Stream buf;

        buf << value;
        node = buf.str();
    }

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
