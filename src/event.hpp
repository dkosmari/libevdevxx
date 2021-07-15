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


#ifndef LIBEVDEVXX_EVENT_HPP
#define LIBEVDEVXX_EVENT_HPP


#include <cstdint>
#include <iosfwd>
#include <string>

#include <libevdev/libevdev.h>


namespace evdev {


    using std::int32_t;
    using std::string;
    using std::uint16_t;
    using std::uint32_t;


    // ---- //
    // Type //
    // ---- //


    enum Type : uint16_t {
        syn       = EV_SYN,
        key       = EV_KEY,
        rel       = EV_REL,
        abs       = EV_ABS,
        msc       = EV_MSC,
        sw        = EV_SW,
        led       = EV_LED,
        snd       = EV_SND,
        rep       = EV_REP,
        ff        = EV_FF,
        pwr       = EV_PWR,
        ff_status = EV_FF_STATUS
    };


    Type to_type(const string& name);

    string to_string(Type type);

    std::ostream& operator<<(std::ostream& out, Type type);


    // ---- //
    // Code //
    // ---- //


    using Code = uint16_t;


    Code count(Type t);


    string to_string(Type t, Code code);



    // ----- //
    // Event //
    // ----- //


    struct Event {
        uint32_t sec{};
        uint32_t usec{};

        Type type{};
        Code code{};
        int32_t value{};


        constexpr Event() noexcept = default;


        constexpr
        Event(const ::input_event& e) noexcept :
            sec{e.input_event_sec},
            usec{e.input_event_usec},
            type{e.type},
            code{e.code},
            value{e.value}
        {}


        constexpr
        Event&
        operator=(const ::input_event& e) noexcept
        {
            sec = e.input_event_sec;
            usec = e.input_event_usec;
            type = Type{e.type};
            code = Code{e.code};
            value = e.value;
            return *this;
        }


        operator ::input_event() const noexcept
        {
            ::input_event r;
            r.input_event_sec = sec;
            r.input_event_usec = usec;
            r.type = type;
            r.code = code;
            r.value = value;
            return r;
        }

    };



    string to_string(const Event& e);


    std::ostream& operator<<(std::ostream& out, const Event& e);


}

#endif
