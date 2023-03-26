/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_UINPUT_HPP
#define LIBEVDEVXX_UINPUT_HPP


#include <filesystem>
#include <memory>
#include <optional>

#include <libevdev/libevdev-uinput.h>

#include "Device.hpp"
#include "Event.hpp"


namespace evdev {


    /// A class to send events to a virtual device.
    class Uinput {

    public:

        Uinput(const Device& dev);
        Uinput(const Device& dev, int filedes);


        ::libevdev_uinput* data() noexcept;
        const ::libevdev_uinput* data() const noexcept;


        int fd() const noexcept;

        std::optional<std::filesystem::path>
        syspath() noexcept;

        std::optional<std::filesystem::path>
        devnode() noexcept;

        void write(Type type, Code code, int value);
        void write(TypeCode type_code, int value);
        void write(const Event& event);


        // convenience methods

        void syn(Code code, int value);
        void key(Code code, int value);
        void rel(Code code, int value);
        void abs(Code code, int value);
        void msc(Code code, int value);
        void sw(Code code, int value);
        void led(Code code, int value);
        void snd(Code code, int value);
        void rep(Code code, int value);
        void ff(Code code, int value);
        void pwr(Code code, int value);
        void ff_status(Code code, int value);

        void flush();


    private:

        struct Deleter {
            void operator()(::libevdev_uinput* udev) const noexcept;
        };

        std::unique_ptr<::libevdev_uinput, Deleter> udev;

    };


} // namespace evdev


#endif
