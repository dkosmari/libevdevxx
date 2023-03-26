/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_CODE_HPP
#define LIBEVDEVXX_CODE_HPP


#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "detail/NumberBase.hpp"
#include "Type.hpp"


namespace evdev {

    /**
     * @brief Type-safe class for evdev codes (`KEY_*`, `BTN_*`, `REL_*`, etc).
     *
     * A `Code` object can only be explicitly constructed from an integer.
     * Conversion back to the integer value is implicit.
     */
    struct Code :
        public detail::NumberBase<std::uint16_t, Code> {

        using Base = detail::NumberBase<std::uint16_t, Code>;

        using typename Base::Value;

        // expose the Base constructors

        using Base::Base;


        Code(Type type,
             Value code_num);


        // named constructors

        /**
         * @brief Parse a `Type` and `Code` from a string
         *
         * @arg `name` the string to parse.
         * @arg[out] `pos` if not NULL, stores how many characters were parsed.
         */
        static
        std::pair<Type, Code>
        parse(std::string_view name,
              std::size_t* pos = nullptr);


        /// Returns the maximum code available for a given type.
        static
        Code
        max(Type type); // depends on type

    };


    /// Converts the code to a string representation.
    std::string
    code_to_string(Type type,
                   Code code);


}


#endif
