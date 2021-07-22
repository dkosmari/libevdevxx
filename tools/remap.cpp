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
#include <iostream>
#include <stdexcept>
#include <thread>
#include <csignal>
#include <map>
#include <functional>

#include <linux/input.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>


#include "evdevxx.hpp"

#include "ini_file.hpp"


using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::filesystem::path;
using std::vector;
using std::string;
using std::exception;
using std::runtime_error;
using std::size_t;
using std::map;
using std::pair;
using std::function;


using namespace std::literals;

using boost::split;
using boost::is_any_of;
using boost::token_compress_on;


using evdev::Device;
using evdev::Uinput;
using evdev::Property;
using evdev::Event;
using Type = Event::Type;
using TypeCode = Event::TypeCode;


using Translator = function<void(const Event&)>;


volatile bool should_quit = false;


map<TypeCode, Translator> translator;


Device
clone(const Device& idev)
{
    Device odev;
    odev.bustype(BUS_VIRTUAL);

    odev.name(idev.name() + " (remap)");
    odev.vendor_id(idev.vendor_id());
    odev.product_id(idev.product_id());
    odev.version(idev.version());

    for (auto& p : idev.properties())
        odev.enable(p);


    return odev;
}


// accept both numeric and symbolic property
Property
parse_property(const string& s)
{
    try {
        size_t pos;
        unsigned long val = std::stoul(s, &pos, 0);
        if (pos == s.size())
            return Property{static_cast<unsigned>(val)};
    }
    catch (exception&) {
    }
    return Property{s};
}


void
set_properties(Device& dev, const IniFile::Entry& entry)
{
    try {
        vector<string> tokens;
        split(tokens, entry.value,
              is_any_of(" ,"),
              token_compress_on);
        for (auto& t : tokens) {
            try {
                switch (t[0]) {
                    case '+':
                        dev.enable(parse_property(t.substr(1)));
                        break;
                    case '-':
                        dev.disable(parse_property(t.substr(1)));
                        break;
                    default:
                        throw runtime_error{"must start with '+' or '-'"};
                }
            }
            catch  (exception& e) {
                throw runtime_error{"line "s + std::to_string(entry.line_num)
                        + " on token '"s
                        + t + "': "s + e.what()};
            }
        }
    }
    catch (exception& e) {
        throw runtime_error{"Properties: "s + e.what()};
    }
}



// translators

void
suppress(const Event&)
{}


Translator
make_forwarder(Uinput& udev)
{
    return [&udev](const Event& e)
    {
        udev.write(e);
    };
}


Translator
make_button_1(Device& dev,
              const string& arg)
{
    // arg must be a key input

}


Translator
make_button_translator(Device& dev, string expr)
{
    boost::trim(expr);
    vector<string> tokens;
    split(tokens, expr, boost::is_space(), token_compress_on);

    switch (tokens.size()) {
        case 0:
            return suppress;
        case 1:
            return make_button_1(dev, tokens.front());
            break;
        default:
            throw runtime_error{"wrong number of arguments in "s
                    + expr};
    }
}


void
config_virtual(Device& dev, const IniFile& config)
{
    auto prop_entry = config.entry("virtual", "properties");
    if (prop_entry)
        set_properties(dev, *prop_entry);

    if (auto group = config.group("map")) {
        for (auto& [key, entry] : *group) {
            auto [type, code] = evdev::to_type_code(key);

            switch (type) {
                case Type::key:
                    translator[{type, code}] =
                        make_button_translator(dev, entry.value);
                    break;
                case Type::abs:
                    // translator[{type, code}] =
                    break;
                default:
                    throw runtime_error{"input code not supported: "s + key};
            }
        }
    }

}


extern "C"
void handle_signal(int)
{
    should_quit = true;
}


int main(int argc, char* argv[])
{
    try {

        if (argc != 3) {
            cerr << "Needs two arguments: <DEVICE> <INI-FILE>" << endl;
            return -1;
        }

        path in_dev_path = argv[1];
        path ini_path = argv[2];

        IniFile config{ini_path};

        // TODO use "[source]" to filter input device
        Device idev{in_dev_path};

        Device odev = clone(idev);

        config_virtual(odev, config);



        Uinput udev{odev};

        std::signal(SIGINT, handle_signal);
        while (!should_quit) {

            std::this_thread::sleep_for(10ms);

        }
        cout << "\nstopping" << endl;

    }
    catch (exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return -1;
    }
}
