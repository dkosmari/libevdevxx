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


#ifndef LIBEVDEVXX_READ_HPP
#define LIBEVDEVXX_READ_HPP

#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {


    // ---- //
    // Flag //
    // ---- //


    enum Flag : unsigned {
        normal     = LIBEVDEV_READ_FLAG_NORMAL,
        resync     = LIBEVDEV_READ_FLAG_SYNC,
        force_sync = LIBEVDEV_READ_FLAG_FORCE_SYNC,
        blocking   = LIBEVDEV_READ_FLAG_BLOCKING
    };


    inline
    constexpr
    Flag
    operator|(Flag a, Flag b) noexcept
    {
        return Flag{ static_cast<unsigned>(a) | static_cast<unsigned>(b) };
    }


    inline
    constexpr
    Flag
    operator&(Flag a, Flag b) noexcept
    {
        return Flag{ static_cast<unsigned>(a) & static_cast<unsigned>(b) };
    }


    inline
    constexpr
    Flag
    operator^(Flag a, Flag b) noexcept
    {
        return Flag{ static_cast<unsigned>(a) ^ static_cast<unsigned>(b) };
    }


    inline
    constexpr
    Flag
    operator~(Flag a) noexcept
    {
        return Flag{ ~static_cast<unsigned>(a) };
    }


    inline
    constexpr
    Flag& operator|=(Flag& a, Flag b)
    {
        a = a | b;
        return a;
    }


    inline
    constexpr
    Flag& operator&=(Flag& a, Flag b)
    {
        a = a & b;
        return a;
    }


    inline
    constexpr
    void set(Flag& f, Flag bit)
    {
        f |= bit;
    }


    inline
    constexpr
    void unset(Flag& f, Flag bit)
    {
        f &= ~bit;
    }


    std::string to_string(Flag flag);

    std::ostream& operator<<(std::ostream& out, Flag flag);



    // ------ //
    // Status //
    // ------ //


    enum Status : int {
        success = LIBEVDEV_READ_STATUS_SUCCESS,
        dropped = LIBEVDEV_READ_STATUS_SYNC,
        again   = -EAGAIN
    };


    std::string to_string(Status st);


    int to_errno(Status st);


    std::ostream& operator<<(std::ostream& out, Status st);

}


#endif
