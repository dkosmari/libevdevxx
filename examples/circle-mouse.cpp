/*
 *  libevdevxx - a C++ wrapper for libevdev
 *  Copyright (C) 2021-2023  Daniel K. O.
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


#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "device.hpp"
#include "uinput.hpp"


using std::cout;
using std::endl;
using std::flush;

using namespace std::literals;

using evdev::Device;
using evdev::Uinput;
using Code = evdev::Event::Code;


int main()
{
    Device dev;
    dev.name("Fake Mouse");

    dev.enable_rel(Code{REL_X});
    dev.enable_rel(Code{REL_Y});
    // needs at least one button
    dev.enable_key(Code{BTN_LEFT});

    Uinput udev{dev};
    cout << "Created uinput device at "
         << udev.devnode().value()
         << endl;
    cout << "Doing a circle... " << flush;

    const float radius = 10;
    for (int i = 0; i < 100; ++i) {
        float angle = i/100.0f * 2 * M_PI;
        int x = static_cast<int>(radius * std::cos(angle));
        int y = static_cast<int>(radius * std::sin(angle));

        udev.rel(Code{REL_X}, x);
        udev.rel(Code{REL_Y}, y);
        udev.flush();

        std::this_thread::sleep_for(16ms);
    }

    cout << "done." << endl;
}
