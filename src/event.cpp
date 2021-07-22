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

#include "private_utils.hpp"


using std::ostream;
using std::ostringstream;
using std::setw;
using std::runtime_error;
using std::out_of_range;
using std::invalid_argument;


using namespace std::literals;


namespace evdev {



    // --------- //
    // Type+Code //
    // --------- //


    Event::TypeCode::TypeCode(const string& type_name,
                              const string& code_name) :
        TypeCode{Type{type_name}, Code{code_name}}
    {}


    Event::TypeCode::TypeCode(const string& code_name)
    {
        int type_val = ::libevdev_event_type_from_code_name(code_name.c_str());
        int code_val = ::libevdev_event_code_from_code_name(code_name.c_str());
        if (type_val == -1 || code_val == -1)
            throw invalid_argument{"bad event code name: '"s
                    + code_name + "'"s};
        type = Type(type_val);
        code = Code(code_val);
    }



    pair<string, string>
    to_string(const Event::TypeCode& tc)
    {
        return {
            to_string(tc.type),
            to_string(tc.type, tc.code)
        };
    }


    ostream&
    operator<<(ostream& out, const Event::TypeCode& tc)
    {
        return out << tc.type << '/' << tc.code;
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
