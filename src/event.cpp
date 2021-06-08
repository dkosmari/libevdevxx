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


#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include "event.hpp"


using std::ostream;
using std::ostringstream;
using std::string;
using std::setw;

using namespace std::literals;


namespace evdev {


    // ---- //
    // Type //
    // ---- //


    Type
    to_type(const string& name)
    {
        int val = ::libevdev_event_type_from_name(name.c_str());
        if (val < 0)
            throw std::runtime_error{"invalid event type name \""s + name + "\""s};
        return Type(val);
    }


    string
    to_string(Type type)
    {
        unsigned val = type;
        const char* s = ::libevdev_event_type_get_name(val);
        if (!s)
            return "unknown event type "s + std::to_string(val);
        return s;
    }


    ostream&
    operator<<(ostream& out, Type type)
    {
        return out << to_string(type);
    }


    Code
    count(Type t)
    {
        switch (t) {
            case Type::syn:
                return SYN_CNT;
            case Type::key:
                return KEY_CNT;
            case Type::rel:
                return REL_CNT;
            case Type::abs:
                return ABS_CNT;
            case Type::sw:
                return SW_CNT;
            case Type::msc:
                return MSC_CNT;
            case Type::led:
                return LED_CNT;
            case Type::rep:
                return REP_CNT;
            case Type::snd:
                return SND_CNT;
            case Type::ff:
                return FF_CNT;
            case Type::ff_status:
                return FF_STATUS_MAX+1;
            default:
                return 0;
        }
    }



    // ---- //
    // Code //
    // ---- //


    string
    to_string(Type t, Code code)
    {
        const char* n = ::libevdev_event_code_get_name(t, code);
        if (!n)
            return "unknown ("s + std::to_string(code) + ")"s;
        return n;
    }



    // ----- //
    // Event //
    // ----- //


    string
    to_string(const Event& e)
    {
        ostringstream out;
        out << e;
        return out.str();
    }


    ostream&
    operator<<(ostream& out, const Event& e)
    {
        out << std::left
            << setw(6) << e.type
            << " / "
            << setw(12) << to_string(e.type, e.code)
            << " = "
            << e.value;
        return out;
    }


}
