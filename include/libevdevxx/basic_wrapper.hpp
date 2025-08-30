/*
 * basic_wrapper - base class for C++ wrappers
 * Copyright 2025  Daniel K. O. (dkosmari)
 *
 * This software can be distributed under the terms of any of the following licenses:
 *   SPDX-License-Identifier: Apache-2.0
 *   SPDX-License-Identifier: GPL-3.0-or-later
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 *   SPDX-License-Identifier: MIT
 *   SPDX-License-Identifier: Zlib
*/

#ifndef LIBEVDEVXX_BASIC_WRAPPER_HPP
#define LIBEVDEVXX_BASIC_WRAPPER_HPP


namespace evdev::detail {

    template<typename T,
             T InvalidValue = T{}>
    class basic_wrapper {

    protected:

        T raw{InvalidValue};


        // Note: we can't call destroy() from the derived class from here.
        ~basic_wrapper()
            noexcept = default;


    public:

        using raw_type = T;
        using state_type = raw_type;

        static constexpr raw_type invalid_value = InvalidValue;


        constexpr
        basic_wrapper()
            noexcept = default;


        constexpr
        explicit
        basic_wrapper(T src)
            noexcept :
            raw{src}
        {}


        /// Move constructor.
        constexpr
        basic_wrapper(basic_wrapper&& other)
            noexcept
        {
            acquire(other.release());
        }


        /// Move assignment.
        basic_wrapper&
        operator =(basic_wrapper&& other)
            noexcept
        {
            if (this != &other) {
                destroy();
                acquire(other.release());
            }
            return *this;
        }


        // Remember to always call destroy() from the derived class' destructor.
        virtual
        void
        destroy()
            noexcept = 0;


        [[nodiscard]]
        bool
        is_valid()
            const noexcept
        {
            return raw != invalid_value;
        }


        [[nodiscard]]
        explicit
        operator bool()
            const noexcept
        {
            return is_valid();
        }


        raw_type
        data()
            noexcept
        {
            return raw;
        }


        const raw_type
        data()
            const noexcept
        {
            return raw;
        }


        void
        acquire(state_type new_state)
            noexcept
        {
            raw = new_state;
        }


        state_type
        release()
            noexcept
        {
            auto old_raw = raw;
            raw = {};
            return old_raw;
        }

    }; // class basic_wrapper

} // namespace evdev::detail

#endif
