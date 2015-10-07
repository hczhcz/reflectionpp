#pragma once

#include "holder.hpp"

namespace rpp {

// to construct accessors as a member of object
template <class T>
struct AccessorFactory {
    // static only
    AccessorFactory() = delete;

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
template <class Value>
struct AccessorSimple: public Value {
    using Value::Value;

    using Type = AccessorSimple<
        HolderType<HoldType<Value>>
    >;
};

// helper class of AccessObject
// add information of object members to an accessor
template <class Value, class Members>
struct AccessorObjectHelper;

template <class Value>
struct AccessorObjectHelper<
    Value, TypeList<>
>: public Value {
    using Value::Value;

    using MemberTypes = TypeList<>;

    rpp_size_t size() {
        return 0;
    }

    const char *getMemberName(rpp_size_t) {
        throw Exception{};
    }

    template <class Visitor>
    typename Visitor::ReturnType doMemberVisit(Visitor &, rpp_size_t) {
        throw Exception{};
    }
};

template <class Value, class Name, class Member, class... Args>
struct AccessorObjectHelper<
    Value, TypeList<Name, Member, Args...>
>: public AccessorObjectHelper<Value, TypeList<Args...>> {
    using AccessorObjectHelper<Value, TypeList<Args...>>::AccessorObjectHelper;

    using MemberTypes = typename AccessorObjectHelper<Value, TypeList<Args...>>
        ::MemberTypes
        ::template Push<typename Member::Type>
        ::template Push<Name>;

    rpp_size_t size() {
        return 1 + AccessorObjectHelper<Value, TypeList<Args...>>::size();
    }

    const char *getMemberName(rpp_size_t index) {
        if (index == 0) {
            // notice: Name is a holder object
            return AccessorFactory<Name>::make((*this)())();
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
template <class Value, class Members>
struct AccessorObject: public AccessorObjectHelper<Value, Members> {
    using AccessorObjectHelper<Value, Members>::AccessorObjectHelper;

    using Type = AccessorObject<
        HolderType<HoldType<Value>>,
        typename AccessorObjectHelper<Value, Members>::MemberTypes
    >;
};

// data accessors associated with dynamic members
template <class Value, class Member>
struct AccessorDynamic: public Value {
    using Value::Value;

    using Type = AccessorDynamic<
        HolderType<HoldType<Value>>,
        typename Member::Type
    >;

    template <class Visitor, class T>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor, T &value) {
        auto member = AccessorFactory<Member>::make(value);

        return visitor(member);
    }
};

}
