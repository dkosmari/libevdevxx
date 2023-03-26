/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include "libevdevxx/TypeCode.hpp"


namespace evdev {


    std::pair<std::string, std::string>
    to_strings(const TypeCode& tc)
    {
        std::string type_name = to_string(tc.type);
        std::string code_name = code_to_string(tc.type, tc.code);
        return { type_name, code_name };
    }


}
