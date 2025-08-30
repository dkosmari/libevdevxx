/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <ostream>

#include "libevdevxx/Event.hpp"

#include "utils.hpp"


using namespace std::literals;


namespace evdev {

    std::string
    to_string(const Event& e)
    {
        using std::to_string;

        return to_string(e.type)
            + ", "s
            + code_to_string(e.type, e.code)
            + " = "s
            + to_string(e.value);

    }


    std::ostream&
    operator <<(std::ostream& out,
                const Event& e)
    {
        return out << to_string(e);
    }

} // namespace evdev
