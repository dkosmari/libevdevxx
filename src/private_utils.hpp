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


#ifndef LIBEVDEVXX_PRIVATE_UTILS_HPP
#define LIBEVDEVXX_PRIVATE_UTILS_HPP


#include <functional>
#include <ios>
#include <iosfwd>
#include <string>


// Note: this is an implementation-side header, do not install.


namespace evdev::priv {

    using std::string;
    using std::to_string;
    using std::size_t;


    using namespace std::literals;


    string to_string(const string& arg);

    string to_string(const char* s);


    template<typename T>
    string
    to_comma_string(const T& arg)
    {
        return ", "s + to_string(arg);
    }


    template<typename T, typename... Rest>
    string
    list_to_string(const T& first, const Rest&... rest)
    {
        return to_string(first) +
            (to_comma_string(rest) + ...);
    }


    string errno_to_string(int e);


    string to_hex(unsigned val, unsigned width = 0, bool base = true);


    std::istream& getline(std::istream& input,
                          string& line,
                          const std::function<bool(char)>& pred);


    class FlagsGuard {
        std::ios_base& stream;
        std::ios_base::fmtflags saved;
    public:
        FlagsGuard(std::ios_base& stream);
        ~FlagsGuard();
    };


    unsigned long
    stoul_range(const string& arg,
                size_t* pos,
                unsigned base,
                unsigned long max,
                const string& error_msg);
}


#endif
