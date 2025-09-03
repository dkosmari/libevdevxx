/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include "libevdevxx/Grabber.hpp"


namespace evdev {

    Grabber::Grabber()
        noexcept = default;


    Grabber::Grabber(Device& d) :
        dev{&d}
    {
        dev->grab();
    }


    Grabber::~Grabber()
    {
        ungrab();
    }


    Grabber::Grabber(Grabber&& other)
        noexcept :
        dev{other.dev}
    {
        other.dev = nullptr;
    }


    Grabber&
    Grabber::operator =(Grabber&& other)
    {
        if (this != &other) {
            ungrab();
            dev = other.dev;
            other.dev = nullptr;
        }
        return *this;
    }


    void
    Grabber::ungrab()
    {
        if (dev) {
            dev->ungrab();
            dev = nullptr;
        }
    }

} // namespace evdev
