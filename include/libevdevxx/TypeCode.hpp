/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_TYPE_CODE_HPP
#define LIBEVDEVXX_TYPE_CODE_HPP


#include <string>
#include <utility>

#include "Code.hpp"
#include "Type.hpp"


namespace evdev {


    struct TypeCode {
        Type type;
        Code code;
    };


    std::pair<std::string, std::string>
    to_strings(const TypeCode& tc);

}


#endif
