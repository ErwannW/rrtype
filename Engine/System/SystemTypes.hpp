//
// Created by np on 11/27/24.
//

#ifndef SYSTEMTYPES_HPP
#define SYSTEMTYPES_HPP

namespace MGE
{
    template<typename... T>
    using SystemFuncPrototype = void(class World&, T&...);

    template<typename... T>
    using SystemFuncPtr = SystemFuncPrototype<T...>*;
}

#endif //SYSTEMTYPES_HPP
