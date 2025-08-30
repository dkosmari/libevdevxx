/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_GRABBER_HPP
#define LIBEVDEVXX_GRABBER_HPP

#include "Device.hpp"


namespace evdev {

    /// RAII class to call Device::grab() and Device::ungrab() on a device.
    class Grabber {

        Device& dev;

    public:

        Grabber(Device& dev);

        ~Grabber();

    }; // class Grabber

} // namespace evdev

#endif
