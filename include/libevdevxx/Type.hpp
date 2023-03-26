/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_TYPE_HPP
#define LIBEVDEVXX_TYPE_HPP


#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <string_view>

#include <libevdev/libevdev.h>

#include "detail/NumberBase.hpp"


namespace evdev {

    class Type :
        public detail::NumberBase<std::uint16_t, Type> {

        using Base = detail::NumberBase<std::uint16_t, Type>;


    public:


        constexpr
        Type()
            noexcept = default;


        explicit
        constexpr
        Type(std::uint16_t number) :
            Base{number}
        {
            if (number > EV_MAX)
                throw std::invalid_argument{"invalid event type number"};
        }


        // named constructors

        static
        Type
        parse(std::string_view name,
              std::size_t* pos = nullptr);


        static
        constexpr
        Type
        max()
            noexcept
        {
            return Type{EV_MAX};
        }


        static
        constexpr
        std::uint16_t
        count()
            noexcept
        {
            return EV_CNT;
        }




        static const Type syn;
        static const Type key;
        static const Type rel;
        static const Type abs;
        static const Type msc;
        static const Type sw;
        static const Type led;
        static const Type snd;
        static const Type rep;
        static const Type ff;
        static const Type pwr;
        static const Type ff_status;


    };


#define DEFINE(x, y)                                            \
    inline constexpr const Type Type::x{EV_ ## y}

    DEFINE(syn, SYN);
    DEFINE(key, KEY);
    DEFINE(rel, REL);
    DEFINE(abs, ABS);
    DEFINE(msc, MSC);
    DEFINE(sw,  SW);
    DEFINE(led, LED);
    DEFINE(snd, SND);
    DEFINE(rep, REP);
    DEFINE(ff,  FF);
    DEFINE(pwr, PWR);
    DEFINE(ff_status, FF_STATUS);

#undef DEFINE


    std::string
    to_string(Type type);


    std::ostream&
    operator <<(std::ostream& out,
                Type type);


} // namespace evdev


#endif
