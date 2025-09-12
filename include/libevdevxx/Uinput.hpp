/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_UINPUT_HPP
#define LIBEVDEVXX_UINPUT_HPP

#include <cstddef>
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

        Uinput(std::nullptr_t p = nullptr)
            noexcept;

        Uinput(const Device& dev,
               int fd = LIBEVDEV_UINPUT_OPEN_MANAGED);

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
        create(const Device& dev,
               int fd = LIBEVDEV_UINPUT_OPEN_MANAGED);

        void
        destroy()
            noexcept override;


        [[nodiscard]]
        int
        get_fd()
            const noexcept;

        [[nodiscard]]
        std::filesystem::path
        get_syspath()
            const;

        [[nodiscard]]
        std::optional<std::filesystem::path>
        try_get_syspath()
            const;

        [[nodiscard]]
        std::filesystem::path
        get_devnode()
            const;

        [[nodiscard]]
        std::optional<std::filesystem::path>
        try_get_devnode()
            const;


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
        write_syn(Code code,
                  int value);

        void
        write_key(Code code,
                  int value);

        void
        write_rel(Code code,
                  int value);

        void
        write_abs(Code code,
                  int value);

        void
        write_msc(Code code,
                  int value);

        void
        write_sw(Code code,
           int value);

        void
        write_led(Code code,
                  int value);

        void
        write_snd(Code code,
                  int value);

        void
        write_rep(Code code,
                  int value);

        void
        write_ff(Code code,
                 int value);

        void
        write_pwr(Code code,
                  int value);

        void
        write_ff_status(Code code,
                        int value);

        void
        flush();

    }; // class Uinput

} // namespace evdev

#endif
