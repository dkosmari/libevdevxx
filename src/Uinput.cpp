/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include "libevdevxx/Uinput.hpp"

#include "error.hpp"


namespace evdev {


    void
    Uinput::Deleter::operator ()(::libevdev_uinput *udev)
        const noexcept
    {
        ::libevdev_uinput_destroy(udev);
    }


    Uinput::Uinput(const Device& dev) :
        Uinput{dev, LIBEVDEV_UINPUT_OPEN_MANAGED}
    {}


    Uinput::Uinput(const Device& dev,
                   int f)
    {
        ::libevdev_uinput* ptr = nullptr;
        int e = ::libevdev_uinput_create_from_device(dev.data(),
                                                     f,
                                                     &ptr);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_uinput_create_from_device()");
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


    std::optional<std::filesystem::path>
    Uinput::syspath() noexcept
    {
        const char* p = ::libevdev_uinput_get_syspath(data());
        if (!p)
            return {};
        return {p};
    }


    std::optional<std::filesystem::path>
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
            throw_sys_error(-e, "from libevdev_uinput_write_event");
    }


    void
    Uinput::write(TypeCode tc, int value)
    { write(tc.type, tc.code, value); }


    void
    Uinput::write(const Event& event)
    {
        write(event.type, event.code, event.value);
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
    { syn(Code{SYN_REPORT}, 0); }

}
