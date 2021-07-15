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

#include "read.hpp"

#include "private_utils.hpp"


using std::ostream;
using std::ostringstream;

using namespace std::literals;


namespace evdev {


    string
    to_string(Flag flag)
    {
        ostringstream out;
        out << flag;
        return out.str();
    }


    ostream&
    operator<<(ostream& out, Flag flag)
    {
        const char* sep = "";
        if (flag & Flag::normal) {
            out << "normal";
            sep = "|";
        }
        if (flag & Flag::resync) {
            out << sep << "resync";
            sep = "|";
        }
        if (flag & Flag::force_sync) {
            out << sep << "force_sync";
            sep = "|";
        }
        if (flag & Flag::blocking)
            out << sep << "blocking";
        return out;
    }



    string
    to_string(Status st)
    {
        switch (st) {
            case Status::success:
                return "success"s;
            case Status::dropped:
                return "dropped"s;
            default:
                return priv::errno_to_string(-static_cast<int>(st));
        }

    }


    ostream&
    operator<<(ostream& out, Status st)
    { return out << to_string(st); }

}
