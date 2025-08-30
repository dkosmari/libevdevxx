/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_NUMBER_BASE_HPP
#define LIBEVDEVXX_NUMBER_BASE_HPP

#include <compare>


namespace evdev::detail {

    // this template uses CRTP to create a type-safe integer


    template<typename T,
             typename D>
    class NumberBase {

    protected:

        using Value = T;
        using Derived = D;

        Value value;

    public:

        constexpr
        NumberBase() noexcept :
            value{0}
        {}


        constexpr
        explicit
        NumberBase(Value value) noexcept :
            value{value}
        {}


        constexpr
        operator Value()
            const noexcept
        {
            return value;
        }


        // comparators


        constexpr
        bool
        operator ==(const NumberBase& other)
            const noexcept = default;

        constexpr
        std::strong_ordering
        operator <=>(const NumberBase& other)
            const noexcept = default;


        constexpr
        NumberBase&
        operator ++() noexcept
        {
            ++value;
            return *this;
        }


        constexpr
        Derived
        operator ++(int) noexcept
        {
            Derived old{*this};
            ++value;
            return old;
        }

    };

} // namespace evdev::detail

#endif
