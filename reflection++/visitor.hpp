#pragma once

namespace rpp {

template <class Nothing = void>
struct VisitorBase {
    using ReturnValue = void;
};

template <class... Args>
struct VisitorList {};

}
