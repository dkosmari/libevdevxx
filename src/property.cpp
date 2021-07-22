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


#include <istream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <iomanip>

#include <libevdev/libevdev.h>

#include "property.hpp"

#include "private_utils.hpp"


using std::ostream;
using std::runtime_error;
using std::invalid_argument;
using std::out_of_range;
using std::istream;

using namespace std::literals;


static_assert(sizeof(evdev::Property) == sizeof(unsigned));


namespace evdev {


    Property::Property(const string& name,
                       bool numeric,
                       size_t* pos)
    {
        if (int val = ::libevdev_property_from_name(name.c_str());
            val >= 0) {
            value = val;
            if (pos)
                *pos = to_string(*this).size();
            return;
        }

        if (!numeric)
            throw invalid_argument{"bad property name: "s + name};

        value = priv::stoul_range(name, pos, 0,
                                  max(),
                                  "bad property number");
    }


    Property::Property(const string& name, bool numeric) :
        Property{name, numeric, nullptr}
    {}


    Property::Property(const string& name, size_t* pos) :
        Property{name, true, pos}
    {}


    Property::Property(const string& name) :
        Property{name, true, nullptr}
    {}



    string
    to_string(Property prop)
    {
        unsigned val = prop;
        const char* s = ::libevdev_property_get_name(val);
        if (!s)
            return priv::to_hex(val, 2);
        return s;
    }


    ostream&
    operator<<(ostream& out, Property prop)
    {
        return out << to_string(prop);
    }


    istream&
    operator>>(istream& input, Property& prop)
    {
        const auto failbit = std::ios_base::failbit;

        istream::sentry sentry{input};
        if (!sentry)
            return input;

        auto is_name_char = [](char c) -> bool
        {
            return std::isupper(c, std::locale::classic())
                || (c == '_');
        };

        if (is_name_char(input.peek())) {
            // read property name
            string token;
            if (priv::getline(input, token, is_name_char))
                try {
                    prop = Property{token};
                }
                catch (std::exception&) {
                    input.setstate(failbit);
                }
            return input;
        } else {
            // read property numeric value with optional base prefix
            priv::FlagsGuard guard{input};
            unsigned value;
            if (input >> std::setbase(0) >> value)
                try {
                    prop = Property(value);
                }
                catch (std::exception&) {
                    input.setstate(failbit);
                }
            return input;
        }

    }

}
