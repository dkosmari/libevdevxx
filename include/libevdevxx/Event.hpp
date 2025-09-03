/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_EVENT_HPP
#define LIBEVDEVXX_EVENT_HPP


#include <compare>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>
#include <utility>

#include <libevdev/libevdev.h>

#include "Code.hpp"
#include "Type.hpp"


namespace evdev {

    struct Event {

        // Same fields as ::input_event, but not same memory layout.
        decltype(::input_event::input_event_sec) sec{};
        decltype(::input_event::input_event_usec) usec{};
        Type type;
        Code code;
        std::int32_t value{};


        constexpr
        Event()
            noexcept = default;


        constexpr
        Event(const ::input_event& e)
            noexcept :
            sec{e.input_event_sec},
            usec{e.input_event_usec},
            type{e.type},
            code{e.code},
            value{e.value}
        {}


        constexpr
        Event&
        operator =(const ::input_event& e)
            noexcept
        {
            sec = e.input_event_sec;
            usec = e.input_event_usec;
            type = Type{e.type};
            code = Code{e.code};
            value = e.value;
            return *this;
        }


        operator ::input_event()
            const noexcept
        {
            ::input_event r;
            r.input_event_sec = sec;
            r.input_event_usec = usec;
            r.type = type;
            r.code = code;
            r.value = value;
            return r;
        }

    }; // class event



    std::string
    to_string(const Event& e);


    std::ostream&
    operator <<(std::ostream& out,
                const Event& e);

} // namespace evdev

#endif
