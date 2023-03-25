/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_EVENT_CODE_HPP
#define LIBEVDEVXX_EVENT_CODE_HPP


#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "detail/NumberBase.hpp"
#include "Type.hpp"


namespace evdev {


    struct Code :
        public detail::NumberBase<std::uint16_t, Code> {

        using Base = detail::NumberBase<std::uint16_t, Code>;

        using typename Base::Value;

        // expose the Base constructors

        using Base::Base;


        Code(Type type,
             Value code_num);


        // named constructors

        static
        std::pair<Type, Code>
        parse(std::string_view name,
              std::size_t* pos = nullptr);


        static
        Code
        max(Type type); // depends on type

    };



    std::string
    code_to_string(Type type,
                   Code code);


}


#endif
