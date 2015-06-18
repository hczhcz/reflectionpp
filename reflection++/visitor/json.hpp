#pragma once

#include <ostream>
#include <string>
#include <memory>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include "../visitor_chain.hpp"
#include "../accessor.hpp"

namespace rpp {

// render a JSON string and write it into a stream
template <class Nothing = void>
struct VisitorJSON: public VisitorBase<void> {
    std::ostream &out;
    rpp_size_t indent;

    VisitorJSON(std::ostream &_out, rpp_size_t _indent = 0):
        out(_out), indent(_indent) {}

private:
    void writeIndent() {
        out << '\n';
        for (rpp_size_t i = 0; i < indent; ++i) {
            out << "    ";
        }
    }

    template <class T>
    void writeChar(T value) {
        switch (value) {
            case '"':
                out << "\\\"";
                break;
            case '\\':
                out << "\\\\";
                break;
            case '/':
                out << "\\/";
                break;
            case '\b':
                out << "\\b";
                break;
            case '\f':
                out << "\\f";
                break;
            case '\n':
                out << "\\n";
                break;
            case '\r':
                out << "\\r";
                break;
            case '\t':
                out << "\\t";
                break;
            default:
                if (value >= 0 && value < 0x1f) {
                    out << "\\u00"
                        << ("01"[value >> 4])
                        << ("0123456789ABCDEF"[value & 0xf]);
                } else {
                    out << value;
                }
                break;
        }
    }

    void visitStr(const char *value) {
        out << '"';

        for (; *value; ++value) {
            writeChar(*value);
        }

        out << '"';
    }

    void visitStr(const wchar_t *value) {
        out << '"';

        for (; *value; ++value) {
            writeChar(*value);
        }

        out << '"';
    }

    template <class... Args>
    void visitStr(const std::basic_string<Args...> &value) {
        out << '"';

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            writeChar(*i);
        }

        out << '"';
    }

    template <class T>
    void visitPtr(T &value) {
        if (value) {
            visit(*value); // TODO
        } else {
            out << "null";
        }
    }

    template <class T>
    void visitArr(T &value) {
        out << '[';

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            if (i != begin) {
                out << ", ";
            }
            visit(*i); // TODO
        }

        out << ']';
    }

    template <class T>
    void visitMap(T &value) {
        out << '{';
        ++indent;

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            if (i != begin) {
                out << ',';
            }
            writeIndent();

            visitStr(i->first);
            out << ": ";
            visit(i->second); // TODO
        }

        --indent;
        writeIndent();
        out << '}';
    }

public:
    void visit(bool &value) { // notice: bool only
        out << (value ? "true" : "false");
    }

    void visit(double value) { // notice: not reference
        out << value;
    }

    // string

    void visit(const char *value) {
        visitStr(value);
    }

    void visit(const wchar_t *value) {
        visitStr(value);
    }

    template <class... Args>
    void visit(const std::basic_string<Args...> &value) {
        visitStr(value);
    }

    // pointer

    template <class... Args>
    void visit(const std::shared_ptr<Args...> &value) {
        visitPtr(value);
    }

    template <class... Args>
    void visit(const std::weak_ptr<Args...> &value) {
        visitPtr(value);
    }

    template <class... Args>
    void visit(const std::unique_ptr<Args...> &value) {
        visitPtr(value);
    }

    template <class T>
    auto visit(const T *value) -> decltype(
        static_cast<void>(
            static_cast<int T::* /* any member pointer */>(nullptr)
        )
    ) {
        visitPtr(value);
    }

    // array

    template <class T, rpp_size_t size>
    void visit(T (&value)[size]) {
        visitArr(value);
    }

    template <class... Args>
    void visit(const std::array<Args...> &value) {
        visitArr(value);
    }

    template <class... Args>
    void visit(const std::deque<Args...> &value) {
        visitArr(value);
    }

    template <class... Args>
    void visit(const std::forward_list<Args...> &value) {
        visitArr(value);
    }

    template <class... Args>
    void visit(const std::list<Args...> &value) {
        visitArr(value);
    }

    template <class... Args>
    void visit(const std::vector<Args...> &value) {
        visitArr(value);
    }

    // map

    template <class... Args>
    void visit(const std::map<Args...> &value) {
        visitMap(value);
    }

    template <class... Args>
    void visit(const std::unordered_map<Args...> &value) {
        visitMap(value);
    }

    template <class Accessor>
    void into(Accessor &accessor) {
        out << '{';
        ++indent;

        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            if (i != 0) {
                out << ',';
            }
            writeIndent();

            visitStr(accessor.getMemberName(i));
            out << ": ";
            accessor.doMemberVisit(*this, i);
        }

        --indent;
        writeIndent();
        out << '}';
    }
};

}
