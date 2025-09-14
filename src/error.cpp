/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <system_error>

#include "error.hpp"


namespace evdev {

    [[noreturn]]
    void
    throw_sys_error(int e)
    {
        std::error_code ec{e, std::system_category()};
        throw std::system_error{ec};
    }


    [[noreturn]]
    void
    throw_sys_error(int e,
                    const std::string& msg)
    {
        std::error_code ec{e, std::system_category()};
        throw std::system_error{ec, msg};
    }

} // namespace evdev
