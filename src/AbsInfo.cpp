/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include "libevdevxx/AbsInfo.hpp"


namespace evdev {


    std::string
    to_string(const AbsInfo& info)
    {
        using std::to_string;

        return "val=" + to_string(info.val)
            + ", min=" + to_string(info.min)
            + ", max=" + to_string(info.max)
            + ", fuzz=" + to_string(info.fuzz)
            + ", flat=" + to_string(info.flat)
            + ", res=" + to_string(info.res);
    }


} // namespace evdev
