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
            throw Error{__PRETTY_FUNCTION__, -e};
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


    opt_path
    Uinput::syspath() noexcept
    {
        const char* p = ::libevdev_uinput_get_syspath(data());
        if (!p)
            return {};
        return {p};
    }


    opt_path
    Uinput::devnode() noexcept
    {
        const char* p = ::libevdev_uinput_get_devnode(data());
        if (!p)
            return {};
        return {p};
    }


    void
    Uinput::write(Type type,
                  Code code,
                  int value)
    {
        int e = ::libevdev_uinput_write_event(data(), type, code, value);
        if (e < 0)
            throw Error{__PRETTY_FUNCTION__, -e};
    }


    void
    Uinput::syn(Code code, int value)
    { write(Type::syn, code, value); }

    void
    Uinput::key(Code code, int value)
    { write(Type::key, code, value); }

    void
    Uinput::rel(Code code, int value)
    { write(Type::rel, code, value); }

    void
    Uinput::abs(Code code, int value)
    { write(Type::abs, code, value); }

    void
    Uinput::msc(Code code, int value)
    { write(Type::msc, code, value); }

    void
    Uinput::sw(Code code, int value)
    { write(Type::sw, code, value); }

    void
    Uinput::led(Code code, int value)
    { write(Type::led, code, value); }

    void
    Uinput::snd(Code code, int value)
    { write(Type::snd, code, value); }

    void
    Uinput::rep(Code code, int value)
    { write(Type::rep, code, value); }

    void
    Uinput::ff(Code code, int value)
    { write(Type::ff, code, value); }

    void
    Uinput::pwr(Code code, int value)
    { write(Type::pwr, code, value); }

    void
    Uinput::ff_status(Code code, int value)
    { write(Type::ff_status, code, value); }


    void
    Uinput::flush()
    { syn(SYN_REPORT, 0); }

}
