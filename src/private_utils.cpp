/*
 *  libevdevxx - a C++ wrapper for libevdev
 *  Copyright (C) 2021  Daniel K. O.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <vector>
#include <string.h>

#include "private_utils.hpp"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


namespace evdev::priv {


    string
    to_string(const string& arg)
    {
        return "\""s + arg + "\""s;
    }


    string
    to_string(const char* s)
    {
        return "\""s + string{s} + "\""s;
    }


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


}
