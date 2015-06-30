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

#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

// render a JSON string and write it into a stream
template <class Out = std::ostream>
struct VisitorJSON: public VisitorBase<void> {
private:
    Out &out;
    rpp_size_t indent;

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

    template <class Accessor, class T>
    void visitPtr(Accessor &accessor, T &value) {
        if (value) {
            accessor.doMemberVisit(*this, *value);
        } else {
            out << "null";
        }
    }

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) {
        out << '[';

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            if (i != begin) {
                out << ", ";
            }
            accessor.doMemberVisit(*this, *i);
        }

        out << ']';
    }

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) {
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
            accessor.doMemberVisit(*this, i->second);
        }

        --indent;
        writeIndent();
        out << '}';
    }

public:
    VisitorJSON(Out &_out, rpp_size_t _indent = 0):
        out{_out}, indent{_indent} {}

    template <class T>
    auto visit(const T value) -> decltype(
        static_cast<void>(
            sizeof(char [2 - static_cast<int>(static_cast<T>(3))])
        )
    ) {
        out << (value ? "true" : "false");
    }

    template <class T>
    auto visit(const T value) -> decltype(
        static_cast<void>(
            sizeof(char [static_cast<int>(static_cast<T>(3)) - 2])
        )
    ) {
        out << std::to_string(value);
    }

    // string

    void visit(const char &value) {
        out << '"';
        writeChar(value);
        out << '"';
    }

    void visit(const wchar_t &value) {
        out << '"';
        writeChar(value);
        out << '"';
    }

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

    template <class Accessor, class T, rpp_size_t size>
    void into(Accessor &accessor, std::array<T, size> &value) {
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

    template <class... Args>
    void operator()(AccessorDynamic<Args...> &accessor) {
        into(accessor, accessor());
    }
};

}
