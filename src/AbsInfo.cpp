/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <ostream>

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


    std::ostream&
    operator <<(std::ostream& out,
                const AbsInfo& info)
    {
        return out << to_string(info);
    }

} // namespace evdev
