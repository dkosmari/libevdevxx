/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include "libevdevxx/Type.hpp"

#include "utils.hpp"


using namespace std::literals;


namespace evdev {


    static_assert(sizeof(Type) == 2);


    Type
    Type::parse(std::string_view name,
                std::size_t* pos)
    {
        int num = ::libevdev_event_type_from_name_n(name.data(),
                                                    name.size());

        if (num == -1)
            throw std::invalid_argument{"bad event type name: "s + std::string{name}};

        Type result{static_cast<std::uint16_t>(num)};
        if (pos)
            *pos = to_string(result).size();

        return result;
    }


    std::string
    to_string(Type type)
    {
        if (const char* s = ::libevdev_event_type_get_name(type))
            return s;
        else
            return priv::to_hex(type, 2);
    }


}
