#pragma once

#include <ostream>
#include <string>
#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

// render a MySQL script and write it into a stream
template <class Out = std::ostream>
struct VisitorMySQLBase: public VisitorBase<void> {
protected:
    Out &out;

    void charEscape(char value) {
        switch (value) {
            case 0:
                out << "\\0";
                break;
            case '\'':
                out << "\\'";
                break;
            case '"':
                out << "\\\"";
                break;
            case '`':
                out << "\\`";
                break;
            case '\b':
                out << "\\b";
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
            case 26:
                out << "\\Z";
                break;
            case '\\':
                out << "\\\\";
                break;
            case '%':
                out << "\\%";
                break;
            case '_':
                out << "\\_";
                break;
            default:
                out << value;
                break;
        }
    }

    void stringEscape(std::string &value) {
        out << '"';

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            charEscape(*i);
        }

        out << '"';
    }

    void identifierEscape(std::string &value) {
        out << '`';

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            charEscape(*i);
        }

        out << '`';
    }

public:
    VisitorMySQLBase(Out &_out): out{_out} {}
};

}
