/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_PROPERTY_HPP
#define LIBEVDEVXX_PROPERTY_HPP

#include <cstddef>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <string_view>

#include <libevdev/libevdev.h>

#include "NumberBase.hpp"


namespace evdev {

    class Property :
        public detail::NumberBase<unsigned, Property> {

        using BaseType = detail::NumberBase<unsigned, Property>;

    public:

        constexpr
        Property()
            noexcept = default;


        constexpr
        Property(unsigned val) :
            BaseType{val}
        {
            if (val > INPUT_PROP_MAX)
                throw std::invalid_argument{"invalid property number"};
        }


        // named constructors
        static
        Property
        parse(std::string_view name,
              std::size_t* pos = nullptr);


        static
        inline
        constexpr
        Property
        max()
            noexcept
        {
            return Property{INPUT_PROP_MAX};
        }


        static const Property pointer;
        static const Property direct;
        static const Property button_pad;
        static const Property semi_mt;
        static const Property top_button_pad;
        static const Property pointing_stick;
        static const Property accelerometer;


    }; // class Property


    inline constexpr Property Property::pointer        {INPUT_PROP_POINTER};
    inline constexpr Property Property::direct         {INPUT_PROP_DIRECT};
    inline constexpr Property Property::button_pad     {INPUT_PROP_BUTTONPAD};
    inline constexpr Property Property::semi_mt        {INPUT_PROP_SEMI_MT};
    inline constexpr Property Property::top_button_pad {INPUT_PROP_TOPBUTTONPAD};
    inline constexpr Property Property::pointing_stick {INPUT_PROP_POINTING_STICK};
    inline constexpr Property Property::accelerometer  {INPUT_PROP_ACCELEROMETER};


    std::string
    to_string(Property prop);


    std::ostream&
    operator <<(std::ostream& out,
                Property prop);


    std::istream&
    operator >>(std::istream& in,
                Property& prop);

} // namespace evdev

#endif
