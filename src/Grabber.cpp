/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include "libevdevxx/Grabber.hpp"


namespace evdev {


    Grabber::Grabber(Device& dev) :
        dev(dev)
    {
        dev.grab();
    }


    Grabber::~Grabber()
    {
        dev.ungrab();
    }


}
