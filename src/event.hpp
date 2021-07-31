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
#include <utility>

#include <libevdev/libevdev.h>

#include "number_base.hpp"


namespace evdev {


    using std::int32_t;
    using std::string;
    using std::uint16_t;
    using std::uint32_t;
    using std::pair;
    using std::size_t;




    // ----- //
    // Event //
    // ----- //


    struct Event {


        class Type;
        class Code;



        // ---- //
        // Type //
        // ---- //


        class Type : public impl::NumberBase<uint16_t, Type> {

            using base_type = impl::NumberBase<uint16_t, Type>;

        public:

            using base_type::base_type;

            Type(const string& name, bool numeric, size_t* pos);
            Type(const string& name,bool numeric);
            Type(const string& name, size_t* pos);
            Type(const string& name);


            static
            constexpr
            Type max() noexcept
            {
                return Type{EV_MAX};
            }


            static
            constexpr
            uint16_t count() noexcept
            {
                return EV_CNT;
            }



            static const Type syn;
            static const Type key;
            static const Type rel;
            static const Type abs;
            static const Type msc;
            static const Type sw;
            static const Type led;
            static const Type snd;
            static const Type rep;
            static const Type ff;
            static const Type pwr;
            static const Type ff_status;


        };



        // ---- //
        // Code //
        // ---- //


        class Code : public impl::NumberBase<uint16_t, Code> {

            using base_type = impl::NumberBase<uint16_t, Code>;

        public:

            using base_type::base_type;

            Code(const string& name, bool numeric, size_t* pos);
            Code(const string& name, bool numeric);
            Code(const string& name, size_t* pos);
            Code(const string& name);


            static
            Code max(Type type);


            string name(Type type) const;
        };



        // --------- //
        // Type+Code //
        // --------- //


        struct TypeCode {
            Type type;
            Code code;

            constexpr TypeCode() noexcept = default;
            constexpr TypeCode(Type type, Code code) noexcept :
                type{type},
                code{code}
            {}

            TypeCode(const string& type_name, const string& code_name);
            TypeCode(const string& code_name);


            constexpr
            bool
            operator==(const TypeCode& other) const noexcept
            {
                return type == other.type
                    && code == other.code;
            }


            constexpr
            bool
            operator!=(const TypeCode& other) const noexcept
            {
                return type != other.type
                    || code != other.code;
            }


            constexpr
            bool
            operator<(const TypeCode& other) const noexcept
            {
                if (type < other.type) return true;
                if (other.type < type) return false;
                return code < other.code;
            }


            constexpr
            bool
            operator<=(const TypeCode& other) const noexcept
            {
                return ! (*this > other);
            }


            constexpr
            bool
            operator>(const TypeCode& other) const noexcept
            {
                if (type > other.type) return true;
                if (other.type > type) return false;
                return code > other.code;
            }


            constexpr
            bool
            operator>=(const TypeCode& other) const noexcept
            {
                return ! (*this < other);
            }

        };




        uint32_t sec{};
        uint32_t usec{};

        Type type;
        Code code;
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

    }; // class Event



    // ---- //
    // Type //
    // ---- //


#define DEFINE(x, y)  \
    constexpr Event::Type Event::Type::x{EV_ ## y}

    DEFINE(syn, SYN);
    DEFINE(key, KEY);
    DEFINE(rel, REL);
    DEFINE(abs, ABS);
    DEFINE(msc, MSC);
    DEFINE(sw,  SW);
    DEFINE(led, LED);
    DEFINE(snd, SND);
    DEFINE(rep, REP);
    DEFINE(ff,  FF);
    DEFINE(pwr, PWR);
    DEFINE(ff_status, FF_STATUS);

#undef DEFINE



    string to_string(Event::Type type);

    std::ostream& operator<<(std::ostream& out, Event::Type type);



    // ---- //
    // Code //
    // ---- //


    string to_string(Event::Type type, Event::Code code);




    // --------- //
    // Type+Code //
    // --------- //


    pair<string, string> to_string(const Event::TypeCode& tc);


    std::ostream& operator<<(std::ostream& out, const Event::TypeCode& tc);




    string to_string(const Event& e);


    std::ostream& operator<<(std::ostream& out, const Event& e);


}

#endif
