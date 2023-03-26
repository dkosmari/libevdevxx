/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <vector>

#include "utils.hpp"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


using std::invalid_argument;
using std::out_of_range;


namespace evdev::priv {


    string errno_to_string(int e)
    {
        std::vector<char> buf(256);
#ifdef STRERROR_R_CHAR_P
        // the GNU C version
        const char* msg = strerror_r(e, buf.data(), buf.size());
        return msg;
#else
        // the POSIX version
        if (strerror_r(e, buf.data(), sizeof buf.size())) {
            buf[0] = '?';
            buf[1] = '\0';
        }
        return buf;
#endif
    }


    string
    to_hex(unsigned val,
           unsigned width,
           bool base)
    {
        std::ostringstream out;
        if (base)
            out << std::showbase;
        out << std::hex
            << std::setw(width)
            << std::setfill('0')
            << val;
        return out.str();
    }


    std::istream&
    getline(std::istream& input,
            string& line,
            const std::function<bool(char)>& pred)
    {
        line.clear();
        char c;
        while (input.get(c)) {
            if (!pred(c)) {
                input.unget();
                break;
            }

            line.push_back(c);
        }
        return input;
    }

}
