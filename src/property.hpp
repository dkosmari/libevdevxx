/*
 *  libevdevxx - a C++ wrapper for libevdev
 *  Copyright (C) 2021  Daniel K. O.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIBEVDEVXX_PROPERTY_HPP
#define LIBEVDEVXX_PROPERTY_HPP


#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>

#include "number_base.hpp"


namespace evdev {

    using std::string;
    using std::size_t;


    class Property : public impl::NumberBase<unsigned, Property> {

        using base_type = impl::NumberBase<unsigned, Property>;

    public:

        using base_type::base_type;


        Property(const string& name, bool numeric, size_t* pos);
        Property(const string& name, bool numeric);
        Property(const string& name, size_t* pos);
        Property(const string& name);


        static
        constexpr
        Property max() noexcept
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


    };


    constexpr Property Property::pointer        {INPUT_PROP_POINTER};
    constexpr Property Property::direct         {INPUT_PROP_DIRECT};
    constexpr Property Property::button_pad     {INPUT_PROP_BUTTONPAD};
    constexpr Property Property::semi_mt        {INPUT_PROP_SEMI_MT};
    constexpr Property Property::top_button_pad {INPUT_PROP_TOPBUTTONPAD};
    constexpr Property Property::pointing_stick {INPUT_PROP_POINTING_STICK};
    constexpr Property Property::accelerometer  {INPUT_PROP_ACCELEROMETER};


    string to_string(Property prop);


    std::ostream& operator<<(std::ostream& out, Property prop);


    std::istream& operator>>(std::istream& in, Property& prop);
}


#endif
