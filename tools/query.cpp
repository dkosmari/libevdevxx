/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
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
#include <libevdevxx/evdevxx.hpp>


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
void
print_if(const string& label,
              const optional<T>& arg)
{
    if (arg)
        cout << label << *arg << "\n";
}


void
print_codes(const Device& d,
                 Type t)
{
    auto codes = d.get_codes(t);
    unsigned x = 4;
    cout << "   ";

    for (size_t i = 0; i < codes.size(); ++i) {
        auto c = codes[i];
        string name = code_to_string(t, c);

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


void
print_abs(const Device& d)
{
    auto codes = d.get_codes(Type::abs);
    for (Code c : codes) {
        cout << "    " << code_to_string(Type::abs, c) << "\n";
        cout << "        " << d.get_abs_info(c) << "\n";
    }
}


void
print_device(const Device& d)
{
    cout << "  Name: " << d.get_name() << "\n";
    print_if("  Phys: ", d.get_phys());
    print_if("  Uniq: ", d.get_uniq());

    cout << hex << setfill('0')
         << "  VID: " << setw(4) << d.get_vendor() << "\n"
         << "  PID: " << setw(4) << d.get_product() << "\n"
         << "  BUS: " << setw(4) << d.get_bustype() << "\n"
         << "  VER: " << setw(4) << d.get_version() << "\n"
         << "  DRV: " << setw(4) << d.get_driver_version() << "\n"
         << dec;

    for (auto prop : d.get_properties())
        cout << "  Prop: " << prop << "\n";

    for (auto t : d.get_types()) {
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


int
main(int argc,
     char* argv[])
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
