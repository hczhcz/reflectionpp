#pragma once

#include "accessor.hpp"

namespace rpp {

// add information of object members to an accessor
template <class Self, class... Args>
struct AccessorObjectHelper;

template <class Self>
struct AccessorObjectHelper<Self>: protected Self {
    using Self::Self;

    rpp_size_t size() {
        return 0;
    }

    template <class Visitor>
    typename Visitor::ReturnValue doObjectVisit(Visitor &visitor) {
        return Self::doRealVisit(visitor);
    }

    template <class Visitor>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor, rpp_size_t index) {
        (void) visitor;
        (void) index;

        throw 1; // TODO
    }
};

template <class Self, class Member, class... Args>
struct AccessorObjectHelper<
    Self, Member, Args...
>: protected AccessorObjectHelper<Self, Args...> {
    using AccessorObjectHelper<Self, Args...>::AccessorObjectHelper;
    using AccessorObjectHelper<Self, Args...>::doObjectVisit;

    rpp_size_t size() {
        return 1 + sizeof...(Args);
    }

    template <class Visitor, rpp_size_t index>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor) {
        if (index == 0) {
            Member member{Self::get()};
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Self, Args...>
                ::template doMemberVisit<Visitor, index - 1>(visitor);
        }
    }

    template <class Visitor>
    typename Visitor::ReturnValue doMemberVisit(Visitor &visitor, rpp_size_t index) {
        if (index == 0) {
            Member member{Self::get()};
            return member.doRealVisit(visitor);
        } else {
            AccessorObjectHelper<Self, Args...>
                ::template doMemberVisit<Visitor>(visitor, index - 1);
        }
    }
};

}
