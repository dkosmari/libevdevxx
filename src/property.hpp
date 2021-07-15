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


#ifndef LIBEVDEVXX_PROPERTY_HPP
#define LIBEVDEVXX_PROPERTY_HPP


#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {

    using std::string;


    enum Property : unsigned {

        pointer        = INPUT_PROP_POINTER,
        direct         = INPUT_PROP_DIRECT,
        button_pad     = INPUT_PROP_BUTTONPAD,
        semi_mt        = INPUT_PROP_SEMI_MT,
        top_button_pad = INPUT_PROP_TOPBUTTONPAD,
        pointing_stick = INPUT_PROP_POINTING_STICK,
        accelerometer  = INPUT_PROP_ACCELEROMETER

    };


    Property to_property(const string& name);

    string to_string(Property prop);

    std::ostream& operator<<(std::ostream& out, Property prop);


}


#endif
