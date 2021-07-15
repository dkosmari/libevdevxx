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


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <exception>
#include <optional>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

// this header is installed as <libevdevxx/evdevxx.hpp>
#include "evdevxx.hpp"


using std::size_t;
using std::cerr;
using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::optional;
using std::setfill;
using std::setw;
using std::string;

using evdev::Device;
using evdev::Property;
using evdev::Type;
using evdev::Code;


unsigned columns = 80;


template<typename T>
void print_if(const string& label, const optional<T>& arg)
{
    if (arg)
        cout << label << *arg << "\n";
}


void print_codes(const Device& d, Type t)
{
    auto codes = d.codes(t);
    unsigned x = 4;
    cout << "   ";

    for (size_t i = 0; i < codes.size(); ++i) {
        auto c = codes[i];
        string name = to_string(t, c);

        if (i > 0 && name.size() + x + 2 > columns) {
            cout << "\n   ";
            x = 3;
        }
        cout << " " << name;
        x += name.size() + 1;
        if (i+1 < codes.size()) {
            cout << ",";
            ++x;
        }
    }
    cout << endl;
}


void print_abs(const Device& d)
{
    auto codes = d.codes(Type::abs);
    for (Code c : codes) {
        cout << "    " << to_string(Type::abs, c) << "\n";
        cout << "        " << d.abs_info(c) << "\n";
    }
}


void print_device(const Device& d)
{
    cout << "  Name: " << d.name() << "\n";
    print_if("  Phys: ", d.phys());
    print_if("  Uniq: ", d.uniq());

    cout << hex << setfill('0')
         << "  VID: " << setw(4) << d.vendor_id() << "\n"
         << "  PID: " << setw(4) << d.product_id() << "\n"
         << "  BUS: " << setw(4) << d.bustype() << "\n"
         << "  VER: " << setw(4) << d.version() << "\n"
         << dec;

    for (auto prop : d.properties())
        cout << "  Prop: " << prop << "\n";

    for (auto t : d.types()) {
        cout << "  Type: " << t << "\n";
        switch (t) {
            case Type::abs:
                print_abs(d);
                break;
            case Type::syn:
                break;
            default:
                print_codes(d, t);
        }
    }

    cout << endl;
}


void
read_columns_env()
{
    winsize w;
    int fd = open("/dev/tty", O_RDONLY);
    if (!ioctl(fd, TIOCGWINSZ, &w))
        columns = w.ws_col;
    close(fd);
}


int main(int argc, char* argv[])
{
    read_columns_env();

    for (int i = 1; i < argc; ++i) {
        try {
            cout << "Device #" << i << ": "
                 << argv[i] << endl;
            print_device(Device{argv[i]});
        }
        catch (std::exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
