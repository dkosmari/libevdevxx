/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_UINPUT_HPP
#define LIBEVDEVXX_UINPUT_HPP

#include <filesystem>
#include <memory>
#include <optional>

#include <libevdev/libevdev-uinput.h>

#include "basic_wrapper.hpp"
#include "Device.hpp"
#include "Event.hpp"


namespace evdev {

    /// A class to send events to a virtual device.
    class Uinput :
        public detail::basic_wrapper<libevdev_uinput*> {

        using BaseType = detail::basic_wrapper<libevdev_uinput*>;

    public:

        Uinput()
            noexcept;

        Uinput(const Device& dev);

        Uinput(const Device& dev,
               int filedes);

        ~Uinput()
            noexcept;


        /// Move constructor.
        Uinput(Uinput&& other)
            noexcept;

        /// Move assignment.
        Uinput&
        operator =(Uinput&& other)
            noexcept;


        void
        destroy()
            noexcept override;


        int
        fd()
            const noexcept;

        std::optional<std::filesystem::path>
        syspath();

        std::optional<std::filesystem::path>
        devnode();


        void
        write(Type type,
              Code code,
              int value);

        void
        write(TypeCode type_code,
              int value);

        void
        write(const Event& event);


        // convenience methods

        void
        syn(Code code,
            int value);

        void
        key(Code code,
            int value);

        void
        rel(Code code,
            int value);

        void
        abs(Code code,
            int value);

        void
        msc(Code code,
            int value);

        void
        sw(Code code,
           int value);

        void
        led(Code code,
            int value);

        void
        snd(Code code,
            int value);

        void
        rep(Code code,
            int value);

        void
        ff(Code code,
           int value);

        void
        pwr(Code code,
            int value);
        void
        ff_status(Code code,
                  int value);

        void
        flush();

    }; // class Uinput

} // namespace evdev

#endif
