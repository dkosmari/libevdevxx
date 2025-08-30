/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <ostream>

#include "libevdevxx/ReadStatus.hpp"
#include "utils.hpp"


using namespace std::literals;


namespace evdev {

    std::string
    to_string(ReadStatus st)
    {
        switch (st) {
            case ReadStatus::success:
                return "success"s;
            case ReadStatus::dropped:
                return "dropped"s;
            default:
                return detail::errno_to_string(-st);
        }

    }


    std::ostream&
    operator <<(std::ostream& out,
                ReadStatus st)
    {
        return out << to_string(st);
    }

} // namespace evdev
