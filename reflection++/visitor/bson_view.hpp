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

#include <bsoncxx/types/value.hpp>
#include <bsoncxx/types.hpp>

#include "../visitor.hpp"
#include "../accessor.hpp"

namespace rpp {

// forward declaration
template <class Element>
struct VisitorBSONViewItemBase;

// read data from a BSON (MongoDB data) view
template <class Base = VisitorBSONViewItemBase<bsoncxx::types::value>>
struct VisitorBSONView: public Base {
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

// read data from an element in a BSON object
template <class Element>
struct VisitorBSONViewItemBase: public VisitorBase<void>, private Element {
private:
    template <class To>
    void castAssign(To &, ...) {
        throw Exception{};
    }

    template <class To, class From>
    auto castAssign(To &to, From *from) -> decltype(
        static_cast<void>(to = *from)
    ) {
        to = *from;
    }

    // bsoncxx::stdx::string_view
    template <class To, class From>
    auto castAssign(To &to, From *from) -> decltype(
        static_cast<void>(to = from->value.to_string())
    ) {
        to = from->value.to_string();
    }

protected:
    template <class T>
    void visitVal(T &value) {
        switch (this->type()) {
            case bsoncxx::type::k_double:
                {
                    auto from = this->get_double();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_utf8:
                {
                    auto from = this->get_utf8();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_document:
                {
                    auto from = this->get_document();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_array:
                {
                    auto from = this->get_array();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_binary:
                {
                    auto from = this->get_binary();
                    castAssign(value, &from);
                }
                break;
            // case bsoncxx::type::k_undefined:
            //     {
            //         auto from = this->get_undefined();
            //         castAssign(value, &from);
            //     }
            //     break;
            case bsoncxx::type::k_oid:
                {
                    auto from = this->get_oid();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_bool:
                {
                    auto from = this->get_bool();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_date:
                {
                    auto from = this->get_date();
                    castAssign(value, &from);
                }
                break;
            case bsoncxx::type::k_null:
                // notice: b_null is discouraged
                {
                    auto from = this->get_null();
                    castAssign(value, &from);
                }
                break;
            // case bsoncxx::type::k_regex:
            //     {
            //         auto from = this->get_regex();
            //         castAssign(value, &from);
            //     }
            //     break;
            // case bsoncxx::type::k_dbpointer:
            //     {
            //         auto from = this->get_dbpointer();
            //         castAssign(value, &from);
            //     }
            //     break;
            // case bsoncxx::type::k_code:
            //     {
            //         auto from = this->get_code();
            //         castAssign(value, &from);
            //     }
            //     break;
            // case bsoncxx::type::k_symbol:
            //     {
            //         auto from = this->get_symbol();
            //         castAssign(value, &from);
            //     }
            //     break;
            // case bsoncxx::type::k_codewscope:
            //     {
            //         auto from = this->get_codewscope();
            //         castAssign(value, &from);
            //     }
            //     break;
            case bsoncxx::type::k_int32:
                {
                    auto from = this->get_int32();
                    castAssign(value, &from);
                }
                break;
            // case bsoncxx::type::k_timestamp:
            //     {
            //         auto from = this->get_timestamp();
            //         castAssign(value, &from);
            //     }
            //     break;
            case bsoncxx::type::k_int64:
                {
                    auto from = this->get_int64();
                    castAssign(value, &from);
                }
                break;
            // case bsoncxx::type::k_maxkey:
            //     {
            //         auto from = this->get_maxkey();
            //         castAssign(value, &from);
            //     }
            //     break;
            // case bsoncxx::type::k_minkey:
            //     {
            //         auto from = this->get_minkey();
            //         castAssign(value, &from);
            //     }
            //     break;
            default:
                throw Exception{};
                break;
        }
    }

    void visitVal(char &value) {
        std::string str;

        visitVal(str);
        value = str[0];
    }

    // void visitVal(wchar_t &value) {
    //     std::wstring str;

    //     visitVal(str);
    //     value = str[0];
    // }

    template <class Self, class Accessor, class T>
    void visitPtr(Self *self, Accessor &accessor, T &value) {
        // notice: special case
        // TODO
        if (this->type() == bsoncxx::type::k_null) {
            // leak?
            value = nullptr;
        } else {
            if (!value) {
                value = T{
                    new typename std::pointer_traits<T>::element_type{}
                };
            }
            accessor.doMemberVisit(*self, *value);
        }

    }

    template <class Accessor, class T>
    void visitArr(Accessor &accessor, T &value) {
        if (this->type() != bsoncxx::type::k_array) {
            throw Exception{};
        }

        auto arr = this->get_array();
        rpp_size_t index = 0;

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i, ++index) {
            auto child_element = arr.value[index];

            VisitorBSONView<
                VisitorBSONViewItemBase<
                    bsoncxx::array::element
                >
            > child{
                child_element.raw,
                child_element.length,
                child_element.offset
            };

            accessor.doMemberVisit(child, *i);
        }
    }

    template <class Accessor, class T>
    void visitMap(Accessor &accessor, T &value) {
        if (this->type() != bsoncxx::type::k_document) {
            throw Exception{};
        }

        auto doc = this->get_document();

        auto begin = std::begin(value);
        auto end = std::end(value);
        for (auto i = begin; i != end; ++i) {
            auto child_element = doc.value[
                bsoncxx::stdx::string_view{i->first}
            ];

            VisitorBSONView<
                VisitorBSONViewItemBase<
                    bsoncxx::document::element
                >
            > child{
                child_element.raw,
                child_element.length,
                child_element.offset
            };

            accessor.doMemberVisit(child, i->second);
        }
    }

    template <class... Args>
    void visitObj(AccessorObject<Args...> &accessor) {
        if (this->type() != bsoncxx::type::k_document) {
            throw Exception{};
        }

        auto doc = this->get_document();

        for (rpp_size_t i = 0; i < accessor.size(); ++i) {
            auto child_element = doc.value[
                bsoncxx::stdx::string_view{accessor.getMemberName(i)}
            ];

            VisitorBSONView<
                VisitorBSONViewItemBase<
                    bsoncxx::document::element
                >
            > child{
                child_element.raw,
                child_element.length,
                child_element.offset
            };

            accessor.doMemberVisit(child, i);
        }
    }

public:
    using Element::Element;
};

}
