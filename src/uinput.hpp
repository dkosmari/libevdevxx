/*
 *  libevdevxx - a C++ wrapper for libevdev
 *  Copyright (C) 2021  Daniel K. O.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LIBEVDEVXX_UINPUT_HPP
#define LIBEVDEVXX_UINPUT_HPP


#include <filesystem>
#include <memory>
#include <optional>

#include <libevdev/libevdev-uinput.h>

#include "device.hpp"
#include "event.hpp"


namespace evdev {


    using std::filesystem::path;
    using std::optional;


    class Uinput {

        struct Deleter {
            void operator()(::libevdev_uinput* udev) const noexcept;
        };

        std::unique_ptr<::libevdev_uinput, Deleter> udev;

    public:

        Uinput(const Device& dev);
        Uinput(const Device& dev, int f);


        ::libevdev_uinput* data() noexcept;
        const ::libevdev_uinput* data() const noexcept;


        int fd() const noexcept;

        optional<path> syspath() noexcept;

        optional<path> devnode() noexcept;

        void write(Event::Type type, Event::Code code, int value);
        void write(Event::TypeCode type_code, int value);
        void write(const Event& event);


        // convenience methods

        void syn(Event::Code code, int value);
        void key(Event::Code code, int value);
        void rel(Event::Code code, int value);
        void abs(Event::Code code, int value);
        void msc(Event::Code code, int value);
        void sw(Event::Code code, int value);
        void led(Event::Code code, int value);
        void snd(Event::Code code, int value);
        void rep(Event::Code code, int value);
        void ff(Event::Code code, int value);
        void pwr(Event::Code code, int value);
        void ff_status(Event::Code code, int value);

        void flush();

    };


}


#endif
