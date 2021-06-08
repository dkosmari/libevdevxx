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


#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <signal.h>
#include <thread>

#include "device.hpp"
#include "dropped_sync.hpp"
#include "error.hpp"


using std::cerr;
using std::cout;
using std::endl;
using std::flush;

using namespace std::literals;

using evdev::Device;
using evdev::DroppedSync;
using evdev::Event;
using evdev::Flag;
using evdev::Status;


volatile std::sig_atomic_t should_quit = false;


extern "C"
void handle_terminate(int)
{
    should_quit = true;
}


int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage:\n"
             << "        dump-events <DEVICE>\n"
             << "<DEVICE> is any of /dev/input/event*" << endl;
        return -1;
    }

    try {
        Device dev{argv[1]};
        cout << "Opened device \"" << dev.name() << "\"" << endl;

        std::signal(SIGINT, handle_terminate);
        std::signal(SIGTERM, handle_terminate);

        Event event;
        while (!should_quit) {

            try {
                while (!should_quit && !dev.pending())
                    std::this_thread::sleep_for(10ms);
                if (should_quit)
                    break;
                Event event = dev.read();
                cout << event << endl;
            }
            catch (DroppedSync& ds) {
                cout << "dropped sync" << endl;
                // trigger a resync
                Event delta;
                while (dev.read(delta, Flag::resync) == Status::dropped)
                    cout << "delta: " << delta << endl;
                cout << "sync restored" << endl;
            }
        }

        cout << "\nExiting." << endl;
    }
    catch (std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

}
