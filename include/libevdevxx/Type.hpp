/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
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

#include "NumberBase.hpp"


namespace evdev {

    class Type :
        public detail::NumberBase<std::uint16_t, Type> {

        using BaseType = detail::NumberBase<std::uint16_t, Type>;

    public:

        constexpr
        Type()
            noexcept = default;


        explicit
        constexpr
        Type(std::uint16_t number) :
            BaseType{number}
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

    }; // class Type


    inline constexpr const Type Type::syn       {EV_SYN};
    inline constexpr const Type Type::key       {EV_KEY};
    inline constexpr const Type Type::rel       {EV_REL};
    inline constexpr const Type Type::abs       {EV_ABS};
    inline constexpr const Type Type::msc       {EV_MSC};
    inline constexpr const Type Type::sw        {EV_SW};
    inline constexpr const Type Type::led       {EV_LED};
    inline constexpr const Type Type::snd       {EV_SND};
    inline constexpr const Type Type::rep       {EV_REP};
    inline constexpr const Type Type::ff        {EV_FF};
    inline constexpr const Type Type::pwr       {EV_PWR};
    inline constexpr const Type Type::ff_status {EV_FF_STATUS};


    std::string
    to_string(Type type);


    std::ostream&
    operator <<(std::ostream& out,
                Type type);

} // namespace evdev

#endif
