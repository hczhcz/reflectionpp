#pragma once

namespace rpp {

using rpp_size_t = unsigned;

// data accessors of simple data
template <class Name, class Value>
struct AccessorSimple: protected Value {
    using Value::Value;

    static const char *getRealName() {
        Name name{};
        return name();
    }

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.visit((*this)());
    }
};

// helper class of AccessObject
// add information of object members to an accessor
template <class Value, class... Args>
struct AccessorObjectHelper;

template <class Value>
struct AccessorObjectHelper<Value>: protected Value {
    using Value::Value;

    rpp_size_t size() {
        return 0;
    }

    template <class Visitor>
    typename Visitor::ReturnType doObjectVisit(Visitor &visitor) {
        return visitor.visit((*this)());
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
    Value, Member, Args...
>: protected AccessorObjectHelper<Value, Args...> {
    using AccessorObjectHelper<Value, Args...>::AccessorObjectHelper;
    using AccessorObjectHelper<Value, Args...>::doObjectVisit;

    rpp_size_t size() {
        return 1 + sizeof...(Args);
    }

    template <class Visitor, rpp_size_t index>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor) {
        if (index == 0) {
            Member member{(*this)()}; // TODO
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Value, Args...>
                ::template doMemberVisit<Visitor, index - 1>(visitor);
        }
    }

    template <class Visitor>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor, rpp_size_t index) {
        if (index == 0) {
            Member member{(*this)()}; // TODO
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Value, Args...>
                ::template doMemberVisit<Visitor>(visitor, index - 1);
        }
    }
};

// data accessors associated with members
template <class Name, class Value, class... Args>
struct AccessorObject: protected AccessorObjectHelper<Value, Args...> {
    using AccessorObjectHelper<Value, Args...>::AccessorObjectHelper;

    static const char *getRealName() {
        Name name{};
        return name();
    }

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.into(
            *static_cast<AccessorObjectHelper<Value, Args...> *>(this)
        );
    }
};

}
