/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include <cassert>
#include <stdexcept>

#include <libevdev/libevdev.h>

#include "libevdevxx/Code.hpp"

#include "utils.hpp"


using namespace std::literals;



namespace evdev {


    static_assert(sizeof(Code) == 2);



    Code::Code(Type type,
               value_type code_num) :
        Base{code_num}
    {
        if (code_num > max(type))
            throw std::invalid_argument{"invalid code value for type "s
                    + to_string(type)};
    }



    std::pair<Type, Code>
    Code::parse(std::string_view name,
                std::size_t* pos)
    {
        int code_num = ::libevdev_event_code_from_code_name_n(name.data(),
                                                              name.size());

        if (code_num == -1)
            throw std::invalid_argument{"bad event code name: "s
                    + std::string{name}};

        int type_num = ::libevdev_event_type_from_code_name_n(name.data(),
                                                              name.size());
        assert(type_num != -1);

        Type type{static_cast<std::uint16_t>(type_num)};
        Code code{static_cast<std::uint16_t>(code_num)};

        if (pos) {
            // we must discover the name, but for that, we must also discover the type
            std::string code_name = code_to_string(type, code);
            *pos = code_name.size();
        }

        return {type, code};
    }


    Code
    Code::max(Type type)
    {
        int val = ::libevdev_event_type_get_max(type);
        if (val == -1)
            throw std::invalid_argument{"bad event type: "s
                    + to_string(type)};
        return Code{static_cast<Value>(val)};
    }



    std::string
    code_to_string(Type type,
                   Code code)
    {
        if (const char* n = ::libevdev_event_code_get_name(type, code))
            return n;
        else
            return priv::to_hex(static_cast<Code::value_type>(code), 3);
    }


}
