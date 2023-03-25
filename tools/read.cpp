/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <signal.h>
#include <thread>

#include <libevdevxx/Device.hpp>
#include <libevdevxx/SyncError.hpp>


using std::cerr;
using std::cout;
using std::endl;
using std::flush;

using namespace std::literals;

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
        evdev::Device dev{argv[1]};
        cout << "Opened device \"" << dev.name() << "\"" << endl;

        std::signal(SIGINT, handle_terminate);
        std::signal(SIGTERM, handle_terminate);

        while (!should_quit) {

            try {
                while (!should_quit && !dev.pending())
                    std::this_thread::sleep_for(10ms);
                if (should_quit)
                    break;

                auto event = dev.read();
                cout << event << endl;
            }
            catch (evdev::SyncError& se) {
                cout << "lost sync" << endl;
                // trigger a resync
                evdev::Event delta;
                while (dev.read(delta, evdev::ReadFlag::resync) == evdev::ReadStatus::dropped)
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
