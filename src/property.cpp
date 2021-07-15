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


#include <ostream>
#include <stdexcept>

#include <libevdev/libevdev.h>

#include "property.hpp"


using std::ostream;
using std::runtime_error;

using namespace std::literals;


namespace evdev {


    Property
    to_property(const string& name)
    {
        int val = ::libevdev_property_from_name(name.c_str());
        if (val < 0)
            throw runtime_error{"invalid property name: "s + name};
        return Property(val);
    }


    string
    to_string(Property prop)
    {
        unsigned val = prop;
        const char* s = ::libevdev_property_get_name(val);
        if (!s)
            return "unknown property "s + std::to_string(val);
        return s;
    }


    ostream&
    operator<<(ostream& out, Property prop)
    {
        return out << to_string(prop);
    }


}
