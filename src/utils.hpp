/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_UTILS_HPP
#define LIBEVDEVXX_UTILS_HPP

#include <functional>
#include <ios>
#include <iosfwd>
#include <string>


// Note: this is an implementation-side header, do not install.


namespace evdev::detail {

    using std::string;
    using std::to_string;
    using std::size_t;


    string
    errno_to_string(int e);


    string
    to_hex(unsigned val,
           unsigned width = 0,
           bool base = true);


    std::istream&
    getline(std::istream& input,
            string& line,
            const std::function<bool(char)>& pred);

} // namespace evdev::detail

#endif
