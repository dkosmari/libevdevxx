/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_ERROR_HPP
#define LIBEVDEVXX_ERROR_HPP


#include <string>


namespace evdev {

    [[noreturn]]
    void
    throw_sys_error(int e);

    [[noreturn]]
    void
    throw_sys_error(int e,
                    const std::string& msg);

} // namespace evdev


#endif
