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

#include <string.h>

#include "error.hpp"

#include "private_utils.hpp"


using std::runtime_error;
using std::string;

using namespace std::literals;


namespace evdev {


    Error::Error(const string& msg, int error) :
        runtime_error{msg + ": "s + priv::errno_to_string(error)},
        error{error}
    {}


    Error::Error(int error) :
        runtime_error{priv::errno_to_string(error)},
        error{error}
    {}


}
