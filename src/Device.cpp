/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <stdexcept>
#include <string>

#ifdef HAVE_CONFIG_
#include <config.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h> // close()
#endif

#include "libevdevxx/Device.hpp"

#include "libevdevxx/Event.hpp"
#include "libevdevxx/SyncError.hpp"

#include "error.hpp"


using std::logic_error;
using std::runtime_error;
using std::to_string;

using namespace std::literals;


namespace evdev {

    void
    Device::log_helper(const libevdev* /*dev*/,
                       libevdev_log_priority priority,
                       void* data,
                       const char* file,
                       int line,
                       const char* func,
                       const char* format,
                       std::va_list args)
        noexcept
    {
        const Device* d = static_cast<const Device*>(data);
        d->log(LogLevel{priority},
               file,
               line,
               func,
               format,
               args);
    }


    // ------------------------- //
    // Initialization and setup. //
    // ------------------------- //


    Device::Device(LogLevel priority)
    {
        create(priority);
    }


    Device::Device(LogLevel priority,
                   int fd)
    {
        create(priority, fd);
    }


    Device::Device(LogLevel priority,
                   const std::filesystem::path& filename,
                   int flags)
    {
        create(priority, filename, flags);
    }


    void
    Device::create(LogLevel priority)
    {
        create(priority, -1);
    }


    void
    Device::create(LogLevel priority,
                   int fd)
    {
        create(priority);
        set_fd(fd);

        auto dev = libevdev_new();
        if (!dev)
            throw runtime_error{"Could not construct libevdev device."};
        if (priority != LogLevel::none) {
            auto pri = static_cast<libevdev_log_priority>(priority);
            libevdev_set_device_log_function(dev,
                                             log_helper,
                                             pri,
                                             this);
        }
        destroy();
        acquire(dev, fd);
    }


    void
    Device::create(LogLevel priority,
                   const std::filesystem::path& filename,
                   int flags)
    {
        try {
            create(priority);
            open(filename, flags);
        }
        catch (...) {
            destroy();
            throw;
        }
    }


    Device::Device(std::nullptr_t)
        noexcept
    {}


    Device::Device()
    {
        create();
    }


    Device::Device(int filedes)
    {
        create(filedes);
    }


    Device::Device(const std::filesystem::path& filename,
                   int flags)
    {
        create(filename, flags);
    }


    Device::~Device()
        noexcept
    {
        destroy();
    }


    void
    Device::create()
    {
        auto ptr = libevdev_new();
        if (!ptr)
            throw runtime_error{"Could not construct libevdev device."};
        destroy();
        acquire(ptr, -1);
    }


    void
    Device::create(int fd)
    {
        libevdev* ptr = nullptr;
        int e = libevdev_new_from_fd(fd, &ptr);
        if (e < 0)
            throw_sys_error(-e, "libevdev_new_from_fd()");
        destroy();
        acquire(ptr, fd);
    }


    void
    Device::create(const std::filesystem::path& filename,
                   int flags)
    {
        int fd = ::open(filename.c_str(), flags);
        if (fd < 1)
            throw_sys_error(errno, "open(\"" + filename.string() + "\")");
        libevdev* ptr = nullptr;
        int e = libevdev_new_from_fd(fd, &ptr);
        if (e < 0) {
            ::close(fd);
            throw_sys_error(-e, "libevdev_new_from_fd()");
        }
        destroy();
        acquire(ptr, fd);
    }


    void
    Device::destroy()
        noexcept
    {
        auto [old_raw, old_fd] = release();
        if (old_raw)
            libevdev_free(old_raw);
        if (old_fd != -1)
            ::close(old_fd);
    }


    Device::Device(Device&& other)
        noexcept
    {
        acquire(other.release());
    }


    Device&
    Device::operator =(Device&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
    }


    void
    Device::acquire(libevdev* new_raw,
                    int new_fd)
        noexcept
    {
        BaseType::acquire(new_raw);
        owned_fd = new_fd;
    }


    void
    Device::acquire(state_type new_state)
        noexcept
    {
        acquire(std::move(std::get<0>(new_state)),
                std::move(std::get<1>(new_state)));
    }


    Device::state_type
    Device::release()
        noexcept
    {
        int old_fd = owned_fd;
        owned_fd = -1;
        return {
            BaseType::release(),
            old_fd
        };
    }


    void
    Device::grab()
    {
        int e = libevdev_grab(raw, LIBEVDEV_GRAB);
        if (e < 0)
            throw_sys_error(-e, "libevdev_grab().");
    }


    void
    Device::ungrab()
    {
        int e = libevdev_grab(raw, LIBEVDEV_UNGRAB);
        if (e < 0)
            throw_sys_error(-e, "libevdev_grab().");
    }


    void
    Device::set_fd(int fd)
    {
        int e = libevdev_set_fd(raw, fd);
        if (e < 0)
            throw_sys_error(-e, "libevdev_set_fd().");
    }


    void
    Device::change_fd(int fd)
    {
        if (libevdev_change_fd(raw, fd))
            throw runtime_error{"libevdev_change_fd()."};
    }


    int
    Device::get_fd()
        const
    {
        int fd = libevdev_get_fd(raw);
        if (fd == -1)
            throw logic_error{"libevdev_get_fd() failed."};
        return fd;
    }


    void
    Device::open(const std::filesystem::path& filename,
                 int flags)
    {
        if (is_open())
            throw logic_error{"File is already open."s};
        owned_fd = ::open(filename.c_str(), flags);
        if (owned_fd < 0)
            throw_sys_error(errno, "Failed to open \""s + filename.string() + "\""s);
        set_fd(owned_fd);
    }


    bool
    Device::is_open()
        const noexcept
    {
        return owned_fd != -1;
    }


    void
    Device::set_nonblock(bool enable)
    {
        if (!is_open())
            throw logic_error{"File must be open before it's set to nonblock."};

        int flags = ::fcntl(owned_fd, F_GETFL);
        if (flags < 0)
            throw_sys_error(errno);

        if (enable)
            flags |= O_NONBLOCK;
        else
            flags &= ~O_NONBLOCK;

        if (::fcntl(owned_fd, F_SETFL, flags) < 0)
            throw_sys_error(errno);
    }


    bool
    Device::get_nonblock()
        const
    {
        if (!is_open())
            throw logic_error{"File must be open before querying the nonblock state."};

        int flags = ::fcntl(owned_fd, F_GETFL);
        if (flags < 0)
            throw_sys_error(errno);

        return flags & O_NONBLOCK;
    }


    // ------------------ //
    // Logging facilities //
    // ------------------ //


    void
    Device::log(LogLevel /*priority*/,
                const char* /*file*/,
                int /*line*/,
                const char* /*func*/,
                const char* /*format*/,
                va_list /*args*/)
        const noexcept
    {}


    // --------------------- //
    // Querying capabilities //
    // --------------------- //


    std::string
    Device::get_name()
        const
    {
        return libevdev_get_name(raw);
    }


    std::optional<std::string>
    Device::get_phys()
        const
    {
        auto p = libevdev_get_phys(raw);
        if (!p)
            return {};
        return {p};
    }


    std::optional<std::string>
    Device::get_uniq()
        const
    {
        auto u = libevdev_get_uniq(raw);
        if (!u)
            return {};
        return {u};
    }


    std::uint16_t
    Device::get_product()
        const noexcept
    {
        return libevdev_get_id_product(raw);
    }


    std::uint16_t
    Device::get_vendor()
        const noexcept
    {
        return libevdev_get_id_vendor(raw);
    }


    std::uint16_t
    Device::get_bustype()
        const noexcept
    {
        return libevdev_get_id_bustype(raw);
    }


    std::uint16_t
    Device::get_version()
        const noexcept
    {
        return libevdev_get_id_version(raw);
    }


    int
    Device::get_driver_version()
        const noexcept
    {
        return libevdev_get_driver_version(raw);
    }


    bool
    Device::has(Property prop)
        const noexcept
    {
        return libevdev_has_property(raw, prop);
    }


    bool
    Device::has(Type type)
        const noexcept
    {
        return libevdev_has_event_type(raw, type);
    }


    bool
    Device::has(Type type,
                Code code)
        const noexcept
    {
        return libevdev_has_event_code(raw, type, code);
    }


    bool
    Device::has(const TypeCode& tc)
        const noexcept
    {
        return has(tc.type, tc.code);
    }


    int
    Device::get_abs_min(Code code)
        const noexcept
    {
        return libevdev_get_abs_minimum(raw, code);
    }


    int
    Device::get_abs_max(Code code)
        const noexcept
    {
        return libevdev_get_abs_maximum(raw, code);
    }


    int
    Device::get_abs_fuzz(Code code)
        const noexcept
    {
        return libevdev_get_abs_fuzz(raw, code);
    }


    int
    Device::get_abs_flat(Code code)
        const noexcept
    {
        return libevdev_get_abs_flat(raw, code);
    }


    int
    Device::get_abs_res(Code code)
        const noexcept
    {
        return libevdev_get_abs_resolution(raw, code);
    }


    AbsInfo
    Device::get_abs_info(Code code)
        const
    {
        auto info = libevdev_get_abs_info(raw, code);
        if (!info)
            throw runtime_error{"libevdev_get_abs_info() failed."};
        return *info;
    }


    int
    Device::get_value(Type type,
                      Code code)
        const noexcept
    {
        return libevdev_get_event_value(raw, type, code);
    }


    int
    Device::get_value(const TypeCode& tc)
        const noexcept
    {
        return get_value(tc.type, tc.code);
    }


    std::optional<int>
    Device::try_get_value(Type type,
                          Code code)
        const noexcept
    {
        int value;
        // returns a boolean, 0 = error
        if (!libevdev_fetch_event_value(raw, type, code, &value))
            return {};
        return value;
    }


    std::optional<int>
    Device::try_get_value(const TypeCode& tc)
        const noexcept
    {
        return try_get_value(tc.type, tc.code);
    }


    bool
    Device::get_repeat(int& delay,
                       int& period)
        const noexcept
    {
        // returns an error code, 0 = success
        int error = libevdev_get_repeat(raw, &delay, &period);
        return !error;
    }


    std::optional<Device::DelayPeriod>
    Device::try_get_repeat()
        const noexcept
    {
        DelayPeriod result;
        if (!get_repeat(result.delay, result.period))
            return {};
        return result;
    }


    // ----------------------------- //
    // Multi-touch related functions //
    // ----------------------------- //


    int
    Device::get_slot(unsigned slot,
                     Code code)
        const noexcept
    {
        return libevdev_get_slot_value(raw, slot, code);
    }


    std::optional<int>
    Device::try_get_slot(unsigned slot,
                         Code code)
        const noexcept
    {
        int value;
        // returns boolean, 0 on error
        if (!libevdev_fetch_slot_value(raw, slot, code, &value))
            return {};
        return value;
    }


    int
    Device::get_num_slots()
        const
    {
        auto result = try_get_num_slots();
        if (!result)
            throw logic_error{"Device as no multi-touch slots."};
        return *result;
    }


    std::optional<int>
    Device::try_get_num_slots()
        const noexcept
    {
        // returns -1 on error
        int r = libevdev_get_num_slots(raw);
        if (r == -1)
            return {};
        return r;
    }


    int
    Device::get_current_slot()
        const noexcept
    {
        return libevdev_get_current_slot(raw);
    }


    // ---------------------------------------- //
    // Modifying the appearance or capabilities //
    // ---------------------------------------- //


    void
    Device::set_name(const std::string& name)
        noexcept
    {
        libevdev_set_name(raw, name.data());
    }


    void
    Device::set_phys(const std::string& phys)
        noexcept
    {
        libevdev_set_phys(raw, phys.data());
    }


    void
    Device::set_uniq(const std::string& uniq)
        noexcept
    {
        libevdev_set_uniq(raw, uniq.data());
    }


    void
    Device::set_product(std::uint16_t pid)
        noexcept
    {
        libevdev_set_id_product(raw, pid);
    }


    void
    Device::set_vendor(std::uint16_t vid)
        noexcept
    {
        libevdev_set_id_vendor(raw, vid);
    }


    void
    Device::set_bustype(std::uint16_t bus)
        noexcept
    {
        libevdev_set_id_bustype(raw, bus);
    }


    void
    Device::set_version(std::uint16_t ver)
        noexcept
    {
        libevdev_set_id_version(raw, ver);
    }


    void
    Device::enable(Property prop)
    {
        if (libevdev_enable_property(raw, prop))
            throw runtime_error{"libevdev_enable_property() failed."};
    }


    void
    Device::disable(Property prop)
    {
        if (libevdev_disable_property(raw, prop))
            throw runtime_error{"libevdev_disable_property() failed."};
    }


    void
    Device::set_value(Type type,
                      Code code,
                      int value)
    {
        if (libevdev_set_event_value(raw, type, code, value))
            throw runtime_error{"libevdev_set_event_value() failed."};
    }


    void
    Device::set_value(const TypeCode& tc,
                      int value)
    {
        return set_value(tc.type, tc.code, value);
    }


    void
    Device::set_slot(unsigned slot,
                     Code code,
                     int value)
    {
        if (libevdev_set_slot_value(raw, slot, code, value))
            throw runtime_error{"libevdev_set_slot_value() failed."};
    }


    void
    Device::set_abs_min(Code code,
                        int val)
        noexcept
    {
        libevdev_set_abs_minimum(raw, code, val);
    }


    void
    Device::set_abs_max(Code code,
                        int val)
        noexcept
    {
        libevdev_set_abs_maximum(raw, code, val);
    }


    void
    Device::set_abs_fuzz(Code code,
                         int val)
        noexcept
    {
        libevdev_set_abs_fuzz(raw, code, val);
    }


    void
    Device::set_abs_flat(Code code,
                         int val)
        noexcept
    {
        libevdev_set_abs_flat(raw, code, val);
    }


    void
    Device::set_abs_res(Code code,
                        int val)
        noexcept
    {
        libevdev_set_abs_resolution(raw, code, val);
    }


    void
    Device::set_abs_info(Code code,
                         const AbsInfo& info)
        noexcept
    {
        ::input_absinfo rinfo = info;
        libevdev_set_abs_info(raw, code, &rinfo);
    }


    void
    Device::enable(Type type)
    {
        if (libevdev_enable_event_type(raw, type))
            throw runtime_error{"libevdev_enable_event_type() failed."};
    }


    void
    Device::disable(Type type)
    {
        if (libevdev_disable_event_type(raw, type))
            throw runtime_error{"libevdev_disable_event_type() failed."};
    }


    void
    Device::enable(Type type,
                   Code code)
    {
        if (type == Type::abs || type == Type::rep)
            throw logic_error{"Wrong overload for type \""
                    + to_string(type) + "\"."};
        if (libevdev_enable_event_code(raw, type, code, nullptr))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::enable(const TypeCode& tc)
    {
        enable(tc.type, tc.code);
    }


    void
    Device::enable_abs(Code code,
                       const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        if (libevdev_enable_event_code(raw, Type::abs, code, &rinfo))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::enable_rep(Code code,
                       int arg)
    {
        if (libevdev_enable_event_code(raw, Type::rep, code, &arg))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::disable(Type type,
                    Code code)
    {
        if (libevdev_disable_event_code(raw, type, code))
            throw runtime_error{"libevdev_disable_event_code() failed."};
    }


    void
    Device::disable(const TypeCode& tc)
    {
        disable(tc.type, tc.code);
    }


    void
    Device::set_kernel_abs_info(Code code,
                                const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        int e = libevdev_kernel_set_abs_info(raw, code, &rinfo);
        if (e < 0)
            throw_sys_error(-e, "libevdev_kernel_set_abs_info()");
    }


    void
    Device::set_kernel_led_value(Code code,
                                 libevdev_led_value value)
    {
        int e = libevdev_kernel_set_led_value(raw, code, value);
        if (e < 0)
            throw_sys_error(-e, "libevdev_kernel_set_led_value()");
    }


    void
    Device::set_clock_id(int clockid)
    {
        int e = libevdev_set_clock_id(raw, clockid);
        if (e < 0)
            throw_sys_error(-e, "libevdev_set_clock_id()");
    }


    // -------------- //
    // Event handling //
    // -------------- //


    Event
    Device::read(ReadFlag flags)
    {
        Event event;
        ReadStatus status = read(event, flags);
        if (status == ReadStatus::dropped && (flags & ReadFlag::resync) == 0)
            throw SyncError{event};

        int e = static_cast<int>(status);
        if (e < 0)
            throw_sys_error(-e, "libevdev_next_event()");

        return event;
    }


    ReadStatus
    Device::read(Event& event,
                 ReadFlag flags)
        noexcept
    {
        ::input_event raw_event;
        int val = libevdev_next_event(raw,
                                        static_cast<unsigned>(flags),
                                        &raw_event);
        if (val >= 0)
            event = raw_event;
        return ReadStatus{val};
    }


    bool
    Device::has_pending()
    {
        int val = libevdev_has_event_pending(raw);
        if (val < 0)
            throw_sys_error(-val, "libevdev_has_event_pending()");
        return val;
    }


    // ------------------- //
    // Convenience methods //
    // ------------------- //


    std::vector<Property>
    Device::get_properties()
        const
    {
        std::vector<Property> result;
        for (unsigned p = 0; p < INPUT_PROP_CNT; ++p)
            if (Property prop{p}; has(prop))
                result.push_back(prop);
        return result;
    }


    std::vector<Type>
    Device::get_types()
        const
    {
        std::vector<Type> result;
        for (auto t = Type{0}; t <= Type::max(); ++t)
            if (has(t))
                result.push_back(t);
        return result;
    }


    std::vector<Code>
    Device::get_codes(Type type)
        const
    {
        return get_codes(type, Code::max(type));
    }


    std::vector<Code>
    Device::get_codes(Type type,
                      Code max)
        const
    {
        std::vector<Code> result;
        for (Code c{0}; c <= max; ++c)
            if (has(type, c))
                result.push_back(c);
        return result;
    }


    void
    Device::enable_key(Code code)
    {
        enable(Type::key, code);
    }


    void
    Device::enable_rel(Code code)
    {
        enable(Type::rel, code);
    }


    void
    Device::enable_abs(Code code)
    {
        enable(Type::abs, code);
    }


} // namespace evdev
