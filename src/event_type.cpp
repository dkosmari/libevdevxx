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
#include <sstream>
#include <stdexcept>

#include "event.hpp"

#include "private_utils.hpp"


using std::ostream;
using std::invalid_argument;

using namespace std::literals;



namespace evdev {


    static_assert(sizeof(Event::Type) == 2);


    Event::Type::Type(const string& name,
                      bool numeric,
                      size_t* pos)
    {
        if (int val = ::libevdev_event_type_from_name(name.c_str());
            val >= 0) {
            value = val;
            if (pos)
                *pos = to_string(*this).size();
            return;
        }

        if (!numeric)
            throw invalid_argument{"bad event type name: "s + name};

        value = priv::stoul_range(name, pos, 0,
                                  max(),
                                  "bad event type number");
    }


    Event::Type::Type(const string& name, bool numeric) :
        Type{name, numeric, nullptr}
    {}



    Event::Type::Type(const string& name, size_t* pos) :
        Type{name, true, pos}
    {}


    Event::Type::Type(const string& name) :
        Type{name, true, nullptr}
    {}


    string
    to_string(Event::Type type)
    {
        const char* s = ::libevdev_event_type_get_name(type);
        if (!s)
            return priv::to_hex(type, 2);
        return s;
    }


    ostream&
    operator<<(ostream& out, Event::Type type)
    {
        return out << to_string(type);
    }

}
