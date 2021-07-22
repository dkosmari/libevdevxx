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


#include "uinput.hpp"

#include "error.hpp"


namespace evdev {


    void
    Uinput::Deleter::operator()(::libevdev_uinput *udev) const noexcept
    {
        ::libevdev_uinput_destroy(udev);
    }


    Uinput::Uinput(const Device& dev) :
        Uinput{dev, LIBEVDEV_UINPUT_OPEN_MANAGED}
    {}


    Uinput::Uinput(const Device& dev, int f)
    {
        ::libevdev_uinput* ptr = nullptr;
        int e = ::libevdev_uinput_create_from_device(dev.data(),
                                                     f,
                                                     &ptr);
        if (e < 0)
            throw Error{"libevdev_uinput_create_from_device()", -e};
        udev.reset(ptr);
    }


    ::libevdev_uinput*
    Uinput::data() noexcept
    {
        return udev.get();
    }


    const ::libevdev_uinput*
    Uinput::data() const noexcept
    {
        return udev.get();
    }


    int
    Uinput::fd() const noexcept
    {
        return ::libevdev_uinput_get_fd(data());
    }


    optional<path>
    Uinput::syspath() noexcept
    {
        const char* p = ::libevdev_uinput_get_syspath(data());
        if (!p)
            return {};
        return {p};
    }


    optional<path>
    Uinput::devnode() noexcept
    {
        const char* p = ::libevdev_uinput_get_devnode(data());
        if (!p)
            return {};
        return {p};
    }


    void
    Uinput::write(Event::Type type,
                  Event::Code code,
                  int value)
    {
        int e = ::libevdev_uinput_write_event(data(), type, code, value);
        if (e < 0)
            throw Error{"libevdev_uinput_write_event", -e};
    }


    void
    Uinput::write(Event::TypeCode tc, int value)
    { write(tc.type, tc.code, value); }


    void
    Uinput::write(const Event& event)
    {
        write(event.type, event.code, event.value);
    }


    void
    Uinput::syn(Event::Code code, int value)
    { write(Event::Type::syn, code, value); }

    void
    Uinput::key(Event::Code code, int value)
    { write(Event::Type::key, code, value); }

    void
    Uinput::rel(Event::Code code, int value)
    { write(Event::Type::rel, code, value); }

    void
    Uinput::abs(Event::Code code, int value)
    { write(Event::Type::abs, code, value); }

    void
    Uinput::msc(Event::Code code, int value)
    { write(Event::Type::msc, code, value); }

    void
    Uinput::sw(Event::Code code, int value)
    { write(Event::Type::sw, code, value); }

    void
    Uinput::led(Event::Code code, int value)
    { write(Event::Type::led, code, value); }

    void
    Uinput::snd(Event::Code code, int value)
    { write(Event::Type::snd, code, value); }

    void
    Uinput::rep(Event::Code code, int value)
    { write(Event::Type::rep, code, value); }

    void
    Uinput::ff(Event::Code code, int value)
    { write(Event::Type::ff, code, value); }

    void
    Uinput::pwr(Event::Code code, int value)
    { write(Event::Type::pwr, code, value); }

    void
    Uinput::ff_status(Event::Code code, int value)
    { write(Event::Type::ff_status, code, value); }


    void
    Uinput::flush()
    { syn(Event::Code{SYN_REPORT}, 0); }

}
