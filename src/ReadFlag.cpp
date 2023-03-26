/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include <ostream>

#include "libevdevxx/ReadFlag.hpp"

#include "utils.hpp"


using namespace std::literals;


namespace evdev {


    std::string
    to_string(ReadFlag flag)
    {
        std::string sep;
        std::string result;

        if (flag & ReadFlag::normal) {
            result += "normal";
            sep = "|";
        }

        if (flag & ReadFlag::resync) {
            result += sep + "resync";
            sep = "|";
        }

        if (flag & ReadFlag::force_sync) {
            result += sep + "force_sync";
            sep = "|";
        }

        if (flag & ReadFlag::blocking)
            result += sep + "blocking";

        return result;
    }


    std::ostream&
    operator <<(std::ostream& out,
                ReadFlag flag)
    {
        return out << to_string(flag);
    }


}
