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

#include "abs_info.hpp"


using std::ostream;
using std::ostringstream;


namespace evdev {


    string
    to_string(const AbsInfo& info)
    {
        ostringstream out;
        out << info;
        return out.str();
    }


    ostream&
    operator<<(ostream& out, const AbsInfo& info)
    {
        out << "val=" << info.val
            << "  min=" << info.min
            << "  max=" << info.max
            << "  fuzz=" << info.fuzz
            << "  flat=" << info.flat
            << "  res=" << info.res;
        return out;
    }


}
