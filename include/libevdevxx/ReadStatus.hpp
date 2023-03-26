/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_READ_STATUS_HPP
#define LIBEVDEVXX_READ_STATUS_HPP


#include <cerrno>
#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {


    enum ReadStatus : int {
        success = LIBEVDEV_READ_STATUS_SUCCESS,
        dropped = LIBEVDEV_READ_STATUS_SYNC,
        again   = -EAGAIN
    };


    std::string
    to_string(ReadStatus st);


    int
    to_errno(ReadStatus st);


    std::ostream&
    operator <<(std::ostream& out,
                ReadStatus st);


} // namespace evdev


#endif
