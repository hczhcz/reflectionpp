#pragma once

namespace rpp {

using rpp_size_t = unsigned;

// data accessors that uses one data holder
template <class Holder>
struct AccessorSimple: protected Holder {
    using Holder::Holder;

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.visit((*this)());
    }
};

// helper class of AccessObject
// add information of object members to an accessor
template <class Holder, class... Args>
struct AccessorObjectHelper;

template <class Holder>
struct AccessorObjectHelper<Holder>: protected Holder {
    using Holder::Holder;

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

template <class Holder, class Member, class... Args>
struct AccessorObjectHelper<
    Holder, Member, Args...
>: protected AccessorObjectHelper<Holder, Args...> {
    using AccessorObjectHelper<Holder, Args...>::AccessorObjectHelper;
    using AccessorObjectHelper<Holder, Args...>::doObjectVisit;

    rpp_size_t size() {
        return 1 + sizeof...(Args);
    }

    template <class Visitor, rpp_size_t index>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor) {
        if (index == 0) {
            Member member{(*this)()}; // TODO
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Holder, Args...>
                ::template doMemberVisit<Visitor, index - 1>(visitor);
        }
    }

    template <class Visitor>
    typename Visitor::ReturnType doMemberVisit(Visitor &visitor, rpp_size_t index) {
        if (index == 0) {
            Member member{(*this)()}; // TODO
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Holder, Args...>
                ::template doMemberVisit<Visitor>(visitor, index - 1);
        }
    }
};

// data accessors associated with members
template <class Holder, class... Args>
struct AccessorObject: protected AccessorObjectHelper<Holder, Args...> {
    using AccessorObjectHelper<Holder, Args...>::AccessorObjectHelper;

    template <class Visitor>
    typename Visitor::ReturnType doRealVisit(Visitor &visitor) {
        return visitor.into(
            *static_cast<AccessorObjectHelper<Holder, Args...> *>(this)
        );
    }
};

}
