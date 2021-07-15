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


#ifndef LIBEVDEVXX_ABS_INFO_HPP
#define LIBEVDEVXX_ABS_INFO_HPP


#include <cstdint>
#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {


    using std::int32_t;
    using std::string;


    struct AbsInfo {
        int32_t val = 0;
        int32_t min = 0;
        int32_t max = 0;
        int32_t fuzz = 0;
        int32_t flat = 0;
        int32_t res = 0;


        constexpr AbsInfo() noexcept = default;


        constexpr
        AbsInfo(const ::input_absinfo& a) noexcept :
            val{a.value},
            min{a.minimum},
            max{a.maximum},
            fuzz{a.fuzz},
            flat{a.flat},
            res{a.resolution}
        {}


        constexpr
        AbsInfo&
        operator=(const ::input_absinfo& a) noexcept
        {
            val = a.value;
            min = a.minimum;
            max = a.maximum;
            fuzz = a.fuzz;
            flat = a.flat;
            res = a.resolution;
            return *this;
        }


        constexpr
        operator ::input_absinfo() const noexcept
        {
            return {
                .value = val,
                .minimum = min,
                .maximum = max,
                .fuzz = fuzz,
                .flat = flat,
                .resolution = res
            };
        }
    };


    string to_string(const AbsInfo& info);


    std::ostream& operator<<(std::ostream& out, const AbsInfo& info);


}


#endif
