/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_ABS_INFO_HPP
#define LIBEVDEVXX_ABS_INFO_HPP


#include <cstdint>
#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {

    /// Maps to the ::input_absinfo struct from `linux/input.h`.
    struct AbsInfo {

        std::int32_t val  = 0;
        std::int32_t min  = 0;
        std::int32_t max  = 0;
        std::int32_t fuzz = 0;
        std::int32_t flat = 0;
        std::int32_t res  = 0;


        constexpr
        AbsInfo()
            noexcept = default;


        constexpr
        AbsInfo(const ::input_absinfo& a)
            noexcept :
            val{a.value},
            min{a.minimum},
            max{a.maximum},
            fuzz{a.fuzz},
            flat{a.flat},
            res{a.resolution}
        {}


        constexpr
        AbsInfo&
        operator=(const ::input_absinfo& a)
            noexcept
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
        operator ::input_absinfo()
            const noexcept
        {
            return {
                .value      = val,
                .minimum    = min,
                .maximum    = max,
                .fuzz       = fuzz,
                .flat       = flat,
                .resolution = res
            };
        }
    };


    std::string
    to_string(const AbsInfo& info);


    std::ostream&
    operator <<(std::ostream& out,
                const AbsInfo& info);


} // namespace evdev


#endif
