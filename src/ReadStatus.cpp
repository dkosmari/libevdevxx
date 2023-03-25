/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
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
                return priv::errno_to_string(-static_cast<int>(st));
        }

    }


    std::ostream&
    operator <<(std::ostream& out,
                ReadStatus st)
    {
        return out << to_string(st);
    }


} // namespace evdev
