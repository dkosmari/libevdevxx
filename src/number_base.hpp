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


#ifndef LIBEVDEVXX_NUMBER_BASE_HPP
#define LIBEVDEVXX_NUMBER_BASE_HPP


#include <string>


namespace evdev::impl {

    using std::string;
    using std::size_t;


    template<typename T,
             typename D>
    class NumberBase {

    protected:

        using number_type = T;
        using derived_type = D;

        number_type value;


    public:


        constexpr
        NumberBase() noexcept :
            value{0}
        {}


        constexpr
        explicit
        NumberBase(number_type value) noexcept :
            value{value}
        {}



        constexpr
        operator number_type() const noexcept
        {
            return value;
        }


        // comparators


        constexpr
        bool
        operator<(const NumberBase& other) const noexcept
        {
            return value < other.value;
        }


        bool
        operator<=(const NumberBase& other) const noexcept
        {
            return value <= other.value;
        }

        constexpr
        bool
        operator>(const NumberBase& other) const noexcept
        {
            return value > other.value;
        }


        bool
        operator>=(const NumberBase& other) const noexcept
        {
            return value >= other.value;
        }


        constexpr
        bool
        operator==(const NumberBase& other) const noexcept
        {
            return value == other.value;
        }


        bool
        operator!=(const NumberBase& other) const noexcept
        {
            return value != other.value;
        }


        NumberBase& operator++() noexcept
        {
            ++value;
            return *this;
        }


        derived_type operator++(int) noexcept
        {
            derived_type old{*this};
            ++value;
            return old;
        }

    };

}

#endif
