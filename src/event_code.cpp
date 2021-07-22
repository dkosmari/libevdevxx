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


#include <stdexcept>


#include "event.hpp"

#include "private_utils.hpp"


using std::invalid_argument;
using std::logic_error;

using namespace std::literals;



namespace evdev {


    static_assert(sizeof(Event::Code) == 2);


    Event::Code::Code(const string& name,
                      bool numeric,
                      size_t* pos)
    {
        if (int val = ::libevdev_event_code_from_code_name(name.c_str());
            val >= 0) {
            value = val;
            if (pos) {
                /*
                 * We can't know how many characters were matched, so we
                 * need to work out the reverse: find the name for this code
                 * and assume this many characters matched.
                 */
                int type = ::libevdev_event_type_from_code_name(name.c_str());
                if (type == -1)
                    throw logic_error{"internal error in libevdev"};
                const char* n = ::libevdev_event_code_get_name(type, value);
                if (!n)
                    throw logic_error{"internal error in libevdev"};
                *pos = string(n).size();
            }
            return;
        }

        if (!numeric)
            throw invalid_argument{"bad event code name: "s + name};

        value = priv::stoul_range(name, pos, 0,
                                  UINT16_MAX,
                                  "bad event code number");
    }


    Event::Code::Code(const string& name, bool numeric) :
        Code{name, numeric, nullptr}
    {}


    Event::Code::Code(const string& name, size_t* pos) :
        Code{name, true, pos}
    {}


    Event::Code::Code(const string& name) :
        Code{name, true, nullptr}
    {}


    Event::Code
    Event::Code::max(Type type)
    {
        int val = ::libevdev_event_type_get_max(type);
        if (val == -1)
            throw invalid_argument{"bad event type: "s + to_string(type)};
        return Code(val);
    }


    string
    Event::Code::name(Type type) const
    {
        if (const char* n = ::libevdev_event_code_get_name(type, value))
            return n;
        else
            return priv::to_hex(value, 3);
    }



    string
    to_string(Event::Type type, Event::Code code)
    { return code.name(type); }


}
