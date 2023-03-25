/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_READ_FLAG_HPP
#define LIBEVDEVXX_READ_FLAG_HPP


#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {


    enum ReadFlag : unsigned {
        normal     = LIBEVDEV_READ_FLAG_NORMAL,
        resync     = LIBEVDEV_READ_FLAG_SYNC,
        force_sync = LIBEVDEV_READ_FLAG_FORCE_SYNC,
        blocking   = LIBEVDEV_READ_FLAG_BLOCKING
    };



    inline
    constexpr
    ReadFlag
    operator |(ReadFlag a,
               ReadFlag b)
        noexcept
    {
        return ReadFlag{ static_cast<unsigned>(a) | static_cast<unsigned>(b) };
    }


    inline
    constexpr
    ReadFlag
    operator &(ReadFlag a,
               ReadFlag b)
        noexcept
    {
        return ReadFlag{ static_cast<unsigned>(a) & static_cast<unsigned>(b) };
    }


    inline
    constexpr
    ReadFlag
    operator ^(ReadFlag a,
               ReadFlag b)
        noexcept
    {
        return ReadFlag{ static_cast<unsigned>(a) ^ static_cast<unsigned>(b) };
    }


    inline
    constexpr
    ReadFlag
    operator ~(ReadFlag a)
        noexcept
    {
        return ReadFlag{ ~static_cast<unsigned>(a) };
    }


    inline
    constexpr
    ReadFlag&
    operator |=(ReadFlag& a,
                ReadFlag b)
    {
        a = a | b;
        return a;
    }


    inline
    constexpr
    ReadFlag&
    operator &=(ReadFlag& a,
                ReadFlag b)
    {
        a = a & b;
        return a;
    }


    inline
    constexpr
    void
    set(ReadFlag& f,
        ReadFlag bit)
    {
        f |= bit;
    }


    inline
    constexpr
    void
    unset(ReadFlag& f,
          ReadFlag bit)
    {
        f &= ~bit;
    }


    std::string
    to_string(ReadFlag flag);


    std::ostream&
    operator <<(std::ostream& out,
                ReadFlag flag);


}


#endif
