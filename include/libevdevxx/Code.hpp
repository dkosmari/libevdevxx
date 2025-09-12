/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_CODE_HPP
#define LIBEVDEVXX_CODE_HPP

#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "NumberBase.hpp"
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

        using BaseType = detail::NumberBase<std::uint16_t, Code>;

        /// The underlying value, same as `std::uint16_t`.
        using value_type = typename BaseType::Value;

        // expose the BaseType constructors
        using BaseType::BaseType;


        Code(Type type,
             value_type code_num);


        // named constructors

        /**
         * @brief Parse a `Type` and `Code` from a string
         *
         * @param name the string to parse.
         * @param[out] pos if not `NULL`, store how many characters were parsed.
         */
        [[nodiscard]]
        static
        std::pair<Type, Code>
        parse(std::string_view name,
              std::size_t* pos = nullptr);


        /// Returns the maximum code available for a given type.
        [[nodiscard]]
        static
        Code
        max(Type type); // depends on type

    };


    /// Converts the code to a string representation.
    [[nodiscard]]
    std::string
    code_to_string(Type type,
                   Code code);

} // namespace evdev

#endif
