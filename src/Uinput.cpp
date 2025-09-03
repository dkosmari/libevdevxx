/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <stdexcept>
#include <utility>

#include "libevdevxx/Uinput.hpp"

#include "error.hpp"


using std::runtime_error;


namespace evdev {

    Uinput::Uinput(std::nullptr_t)
        noexcept
    {}


    Uinput::Uinput(const Device& dev,
                   int fd)
    {
        create(dev, fd);
    }


    Uinput::~Uinput()
        noexcept
    {
        destroy();
    }


    Uinput::Uinput(Uinput&& other)
        noexcept = default;


    Uinput&
    Uinput::operator =(Uinput&& other)
        noexcept = default;



    void
    Uinput::create(const Device& dev,
                   int fd)
    {
        libevdev_uinput* udev = nullptr;
        int e = libevdev_uinput_create_from_device(dev.data(),
                                                   fd,
                                                   &udev);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_uinput_create_from_device()");
        destroy();
        acquire(udev);
    }


    void
    Uinput::destroy()
        noexcept
    {
        auto old_raw = BaseType::release();
        if (old_raw)
            libevdev_uinput_destroy(old_raw);
    }


    int
    Uinput::get_fd()
        const noexcept
    {
        return libevdev_uinput_get_fd(raw);
    }


    std::filesystem::path
    Uinput::get_syspath()
        const
    {
        auto result = try_get_syspath();
        if (!result)
            throw runtime_error{"No syspath for this uinput device."};
        return std::move(*result);
    }


    std::optional<std::filesystem::path>
    Uinput::try_get_syspath()
        const
    {
        const char* p = libevdev_uinput_get_syspath(raw);
        if (!p)
            return {};
        return std::filesystem::path{p};
    }


    std::filesystem::path
    Uinput::get_devnode()
        const
    {
        auto result = try_get_devnode();
        if (!result)
            throw runtime_error{"No devnode for this uinput device."};
        return std::move(*result);
    }


    std::optional<std::filesystem::path>
    Uinput::try_get_devnode()
        const
    {
        const char* p = libevdev_uinput_get_devnode(raw);
        if (!p)
            return {};
        return std::filesystem::path{p};
    }


    void
    Uinput::write(Type type,
                  Code code,
                  int value)
    {
        int e = libevdev_uinput_write_event(raw, type, code, value);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_uinput_write_event");
    }


    void
    Uinput::write(TypeCode tc,
                  int value)
    {
        write(tc.type, tc.code, value);
    }


    void
    Uinput::write(const Event& event)
    {
        write(event.type, event.code, event.value);
    }


    void
    Uinput::write_syn(Code code,
                      int value)
    {
        write(Type::syn, code, value);
    }

    void
    Uinput::write_key(Code code,
                      int value)
    {
        write(Type::key, code, value);
    }

    void
    Uinput::write_rel(Code code,
                      int value)
    {
        write(Type::rel, code, value);
    }

    void
    Uinput::write_abs(Code code,
                      int value)
    {
        write(Type::abs, code, value);
    }

    void
    Uinput::write_msc(Code code,
                      int value)
    {
        write(Type::msc, code, value);
    }

    void
    Uinput::write_sw(Code code,
                     int value)
    {
        write(Type::sw, code, value);
    }


    void
    Uinput::write_led(Code code,
                      int value)
    {
        write(Type::led, code, value);
    }


    void
    Uinput::write_snd(Code code,
                      int value)
    {
        write(Type::snd, code, value);
    }


    void
    Uinput::write_rep(Code code,
                      int value)
    {
        write(Type::rep, code, value);
    }


    void
    Uinput::write_ff(Code code,
                     int value)
    {
        write(Type::ff, code, value);
    }


    void
    Uinput::write_pwr(Code code,
                      int value)
    {
        write(Type::pwr, code, value);
    }


    void
    Uinput::write_ff_status(Code code,
                            int value)
    {
        write(Type::ff_status, code, value);
    }


    void
    Uinput::flush()
    {
        write_syn(Code{SYN_REPORT}, 0);
    }

} // namespace evdev
