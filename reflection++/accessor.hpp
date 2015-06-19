#pragma once

#include "type_list.hpp"
#include "holder.hpp"

namespace rpp {

using rpp_size_t = unsigned long;

// to construct accessors as a member
template <class T>
struct AccessorFactory {
    template <class Object, class Accessor = T>
    static auto make(Object &) -> decltype(Accessor{}) {
        return Accessor{};
    }

    template <class Object, class Accessor = T>
    static auto make(Object &object) -> decltype(Accessor{object}) {
        return Accessor{object};
    }
};

// data accessors of simple data
template <class Name, class Value>
struct AccessorSimple: protected Value {
    using Value::Value;

    using Meta = AccessorSimple<
        Name, HolderType<decltype((*static_cast<Value *>(nullptr))())>
    >;

    const char *getRealName() {
        return AccessorFactory<Name>::make((*this)())();
    }

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.visit((*this)());
    }
};

// helper class of AccessObject
// add information of object members to an accessor
template <class Value, class Members>
struct AccessorObjectHelper;

template <class Value>
struct AccessorObjectHelper<
    Value, TypeList<>
>: protected Value {
    using Value::Value;

    using MetaList = TypeList<>;

    rpp_size_t size() {
        return 0;
    }

    const char *getMemberName(rpp_size_t index) {
        (void) index;

        throw 1; // TODO
    }

    template <class Visitor>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor, rpp_size_t index) {
        (void) visitor;
        (void) index;

        throw 1; // TODO
    }
};

template <class Value, class Member, class... Args>
struct AccessorObjectHelper<
    Value, TypeList<Member, Args...>
>: protected AccessorObjectHelper<Value, TypeList<Args...>> {
    using AccessorObjectHelper<Value, TypeList<Args...>>::AccessorObjectHelper;

    using MetaList = typename AccessorObjectHelper<Value, TypeList<Args...>>
        ::MetaList
        ::template Push<typename Member::Meta>;

    rpp_size_t size() {
        return 1 + sizeof...(Args);
    }

    const char *getMemberName(rpp_size_t index) {
        if (index == 0) {
            return AccessorFactory<Member>::make((*this)()).getRealName();
        } else {
            return AccessorObjectHelper<Value, TypeList<Args...>>
                ::getMemberName(index - 1);
        }
    }

    template <class Visitor>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor, rpp_size_t index) {
        if (index == 0) {
            // notice: can not use universal init here
            auto member = AccessorFactory<Member>::make((*this)());

            return visitor(member);
        } else {
            return AccessorObjectHelper<Value, TypeList<Args...>>
                ::template doMemberVisit<Visitor>(visitor, index - 1);
        }
    }
};

// data accessors associated with members
template <class Name, class Value, class Members>
struct AccessorObject: public AccessorObjectHelper<Value, Members> {
    using AccessorObjectHelper<Value, Members>::AccessorObjectHelper;

    using Meta = AccessorObject<
        Name, HolderType<decltype((*static_cast<Value *>(nullptr))())>,
        typename AccessorObjectHelper<Value, Members>::MetaList
    >;

    const char *getRealName() {
        return AccessorFactory<Name>::make((*this)())();
    }

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.visit((*this)());
    }
};

}
