/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <istream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <iomanip>

#include <libevdev/libevdev.h>

#include "libevdevxx/Property.hpp"

#include "utils.hpp"


using namespace std::literals;


static_assert(sizeof(evdev::Property) == sizeof(unsigned));


namespace evdev {


    Property
    Property::parse(std::string_view name,
                    std::size_t* pos)
    {
        int val = ::libevdev_property_from_name_n(name.data(),
                                                  name.size());

        if (val == -1)
            throw std::invalid_argument{"bad property name: "s
                    + std::string{name}};

        Property prop{static_cast<Value>(val)};

        if (pos)
            *pos = to_string(prop).size();

        return prop;
    }



    std::string
    to_string(Property prop)
    {
        unsigned val = prop;
        if (const char* s = ::libevdev_property_get_name(val))
            return s;
        else
            return priv::to_hex(val, 2);
    }


    std::ostream&
    operator <<(std::ostream& out,
                Property prop)
    {
        return out << to_string(prop);
    }


    std::istream&
    operator >>(std::istream& input,
                Property& prop)
    {
        const auto failbit = std::ios_base::failbit;

        std::istream::sentry sentry{input};
        if (!sentry)
            return input;

        auto is_valid = [](char c) -> bool
        {
            return std::isupper(c, std::locale::classic())
                || (c == '_');
        };

        if (!is_valid(input.peek())) {
            input.setstate(failbit);
            return input;
        }

        // read property name
        std::string token;
        if (priv::getline(input, token, is_valid))
            try {
                std::size_t sz = 0;
                prop = Property::parse(token, &sz);
                if (sz != token.size())
                    input.setstate(failbit);
            }
            catch (std::exception&) {
                input.setstate(failbit);
            }

        return input;

    }

}
