/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#include <stdexcept>
#include <string>

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
    Device::Deleter::operator ()(::libevdev* dev) const noexcept
    {
        ::libevdev_free(dev);
    }



    namespace {

        void device_log_helper(const ::libevdev* /*dev*/,
                               ::libevdev_log_priority priority,
                               void* data,
                               const char* file,
                               int line,
                               const char* func,
                               const char* format,
                               std::va_list args)
        {
            const Device* d = static_cast<const Device*>(data);
            d->log(LogPriority{priority},
                   file,
                   line,
                   func,
                   format,
                   args);
        }

    }


    // ------------------------- //
    // Initialization and setup. //
    // ------------------------- //

    Device::Device(LogPriority priority) :
        dev{::libevdev_new()}
    {
        if (priority != LogPriority::none) {
            auto p = static_cast<::libevdev_log_priority>(priority);
            libevdev_set_device_log_function(data(),
                                             device_log_helper,
                                             p,
                                             this);
        }
    }


    Device::Device(LogPriority priority,
                   int filedes) :
        Device{priority}
    {
        fd(filedes);
    }


    Device::Device(LogPriority priority,
                   const std::filesystem::path& filename,
                   int flags) :
        Device{priority}
    {
        owned_file.open(filename, flags);
        fd(owned_file.handle());
    }


    Device::Device() :
        dev{::libevdev_new()}
    {
        if (!dev)
            throw std::runtime_error{"Could not construct libevdev device."};

    }


    Device::Device(int filedes)
    {
        ::libevdev* ptr = nullptr;
        int e = ::libevdev_new_from_fd(filedes, &ptr);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_new_from_fd()");
        dev.reset(ptr);
    }


    Device::Device(const std::filesystem::path& filename,
                   int flags) :
        owned_file{filename, flags}
    {
        ::libevdev* ptr = nullptr;
        int e = ::libevdev_new_from_fd(owned_file.handle(), &ptr);
        if (e < 0)
            throw_sys_error(-e, "libevdev_new_from_fd()");
        dev.reset(ptr);
    }


    Device::~Device() noexcept = default;


    libevdev*
    Device::data() noexcept
    {
        return dev.get();
    }


    const ::libevdev*
    Device::data() const noexcept
    {
        return dev.get();
    }


    void
    Device::grab()
    {
        int e = ::libevdev_grab(data(), LIBEVDEV_GRAB);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_grab().");
    }


    void
    Device::ungrab()
    {
        int e = ::libevdev_grab(data(), LIBEVDEV_UNGRAB);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_grab().");
    }


    void
    Device::fd(int f)
    {
        int e = ::libevdev_set_fd(data(), f);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_set_fd().");
    }


    void
    Device::change_fd(int f)
    {
        if (::libevdev_change_fd(data(), f))
            throw runtime_error{"from libevdev_change_fd()."};
    }


    int
    Device::fd() const
    {
        int f = ::libevdev_get_fd(data());
        if (f == -1)
            throw logic_error{"libevdev_get_fd() failed."};
        return f;
    }


    void
    Device::open(const std::filesystem::path& filename,
                 int flags)
    {
        owned_file.open(filename, flags);
    }


    bool
    Device::is_open() const noexcept
    {
        return owned_file.is_open();
    }


    void
    Device::close() noexcept
    {
        owned_file.close();
    }


    void
    Device::nonblock(bool enable)
    {
        owned_file.nonblock(enable);
    }


    // ------------------ //
    // Logging facilities //
    // ------------------ //
    void
    Device::log(LogPriority,
                const char* /*file*/, int /*line*/, const char* /*func*/,
                const char* /*format*/, va_list /*args*/) const
    {}



    // --------------------- //
    // Querying capabilities //
    // --------------------- //


    std::string
    Device::name() const
    { return ::libevdev_get_name(data()); }


    std::optional<std::string>
    Device::phys() const
    {
        auto p = ::libevdev_get_phys(data());
        if (!p)
            return {};
        return {p};
    }


    std::optional<std::string>
    Device::uniq() const
    {
        auto u = ::libevdev_get_uniq(data());
        if (!u)
            return {};
        return {u};
    }


    std::uint16_t
    Device::product() const noexcept
    { return ::libevdev_get_id_product(data()); }


    std::uint16_t
    Device::vendor() const noexcept
    { return ::libevdev_get_id_vendor(data()); }


    std::uint16_t
    Device::bustype() const noexcept
    { return ::libevdev_get_id_bustype(data()); }


    std::uint16_t
    Device::version() const noexcept
    { return ::libevdev_get_id_version(data()); }


    int
    Device::driver_version() const noexcept
    { return ::libevdev_get_driver_version(data()); }


    bool
    Device::has(Property prop) const noexcept
    { return ::libevdev_has_property(data(), prop); }


    bool
    Device::has(Type type) const noexcept
    { return ::libevdev_has_event_type(data(), type); }


    bool
    Device::has(Type type, Code code) const noexcept
    { return ::libevdev_has_event_code(data(), type, code); }


    bool
    Device::has(const TypeCode& tc) const noexcept
    { return has(tc.type, tc.code); }


    int
    Device::abs_min(Code code) const noexcept
    { return ::libevdev_get_abs_minimum(data(), code); }


    int
    Device::abs_max(Code code) const noexcept
    { return ::libevdev_get_abs_maximum(data(), code); }


    int
    Device::abs_fuzz(Code code) const noexcept
    { return ::libevdev_get_abs_fuzz(data(), code); }


    int
    Device::abs_flat(Code code) const noexcept
    { return ::libevdev_get_abs_flat(data(), code); }


    int
    Device::abs_res(Code code) const noexcept
    { return ::libevdev_get_abs_resolution(data(), code); }


    AbsInfo
    Device::abs_info(Code code) const
    {
        auto info = ::libevdev_get_abs_info(data(), code);
        if (!info)
            throw runtime_error{"libevdev_get_abs_info() failed."};
        return *info;
    }


    int
    Device::get(Type type, Code code) const noexcept
    { return ::libevdev_get_event_value(data(), type, code); }


    int
    Device::get(const TypeCode& tc) const noexcept
    { return get(tc.type, tc.code); }


    std::optional<int>
    Device::fetch(Type type, Code code) const noexcept
    {
        int value;
        // returns a boolean, 0 = error
        if (!::libevdev_fetch_event_value(data(), type, code, &value))
            return {};
        return {value};
    }


    std::optional<int>
    Device::fetch(const TypeCode& tc) const noexcept
    { return fetch(tc.type, tc.code); }


    bool
    Device::get_repeat(int& delay, int& period) const noexcept
    {
        // returns an error code, 0 = success
        int error = ::libevdev_get_repeat(data(), &delay, &period);
        return !error;
    }



    // ----------------------------- //
    // Multi-touch related functions //
    // ----------------------------- //


    int
    Device::get_slot(unsigned slot, Code code) const noexcept
    {
        return ::libevdev_get_slot_value(data(), slot, code);
    }


    std::optional<int>
    Device::fetch_slot(unsigned slot, Code code) const noexcept
    {
        int value;
        // returns boolean, 0 on error
        if (!::libevdev_fetch_slot_value(data(), slot, code, &value))
            return {};
        return {value};
    }


    std::optional<int>
    Device::num_slots() const noexcept
    {
        // returns -1 on error
        int r = ::libevdev_get_num_slots(data());
        if (r == -1)
            return {};
        return {r};
    }


    int
    Device::current_slot() const noexcept
    { return ::libevdev_get_current_slot(data()); }



    // ---------------------------------------- //
    // Modifying the appearance or capabilities //
    // ---------------------------------------- //


    void
    Device::name(const std::string& n)
    { ::libevdev_set_name(data(), n.c_str()); }


    void
    Device::phys(const std::string& p)
    { ::libevdev_set_phys(data(), p.c_str()); }


    void
    Device::uniq(const std::string& u)
    { ::libevdev_set_uniq(data(), u.c_str()); }


    void
    Device::product(std::uint16_t pid)
    { ::libevdev_set_id_product(data(), pid); }


    void
    Device::vendor(std::uint16_t vid)
    { ::libevdev_set_id_vendor(data(), vid); }


    void
    Device::bustype(std::uint16_t bus)
    { ::libevdev_set_id_bustype(data(), bus); }


    void
    Device::version(std::uint16_t ver)
    { ::libevdev_set_id_version(data(), ver); }


    void
    Device::enable(Property prop)
    {
        if (::libevdev_enable_property(data(), prop))
            throw runtime_error{"libevdev_enable_property() failed."};
    }


    void
    Device::disable(Property prop)
    {
        if (::libevdev_disable_property(data(), prop))
            throw runtime_error{"libevdev_disable_property() failed."};
    }


    void
    Device::set(Type type, Code code, int value)
    {
        if (::libevdev_set_event_value(data(), type, code, value))
            throw runtime_error{"libevdev_set_event_value() failed."};
    }


    void
    Device::set(const TypeCode& tc, int value)
    { return set(tc.type, tc.code, value); }


    void
    Device::set_slot(unsigned slot, Code code, int value)
    {
        if (::libevdev_set_slot_value(data(), slot, code, value))
            throw runtime_error{"libevdev_set_slot_value() failed."};
    }


    void
    Device::abs_min(Code code, int val)
    { ::libevdev_set_abs_minimum(data(), code, val); }


    void
    Device::abs_max(Code code, int val)
    { ::libevdev_set_abs_maximum(data(), code, val); }


    void
    Device::abs_fuzz(Code code, int val)
    { ::libevdev_set_abs_fuzz(data(), code, val); }


    void
    Device::abs_flat(Code code, int val)
    { ::libevdev_set_abs_flat(data(), code, val); }


    void
    Device::abs_res(Code code, int val)
    { ::libevdev_set_abs_resolution(data(), code, val); }


    void
    Device::abs_info(Code code, const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        ::libevdev_set_abs_info(data(), code, &rinfo);
    }


    void
    Device::enable(Type type)
    {
        if (::libevdev_enable_event_type(data(), type))
            throw runtime_error{"libevdev_enable_event_type() failed."};
    }


    void
    Device::disable(Type type)
    {
        if (::libevdev_disable_event_type(data(), type))
            throw runtime_error{"libevdev_disable_event_type() failed."};
    }


    void
    Device::enable(Type type,
                   Code code)
    {
        if (type == Type::abs || type == Type::rep)
            throw logic_error{"Wrong overload for type \""
                    + to_string(type) + "\"."};
        if (::libevdev_enable_event_code(data(), type, code, nullptr))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::enable(const TypeCode& tc)
    { enable(tc.type, tc.code); }


    void
    Device::enable_abs(Code code,
                       const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        if (::libevdev_enable_event_code(data(), Type::abs, code, &rinfo))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::enable_rep(Code code,
                       int arg)
    {
        if (::libevdev_enable_event_code(data(), Type::rep, code, &arg))
            throw runtime_error{"libevdev_enable_event_code() failed."};
    }


    void
    Device::disable(Type type,
                    Code code)
    {
        if (::libevdev_disable_event_code(data(), type, code))
            throw runtime_error{"libevdev_disable_event_code() failed."};
    }


    void
    Device::disable(const TypeCode& tc)
    { disable(tc.type, tc.code); }


    void
    Device::kernel_abs_info(Code code, const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        int e = ::libevdev_kernel_set_abs_info(data(), code, &rinfo);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_kernel_set_abs_info()");
    }


    void
    Device::kernel_led_value(Code code, ::libevdev_led_value value)
    {
        int e = ::libevdev_kernel_set_led_value(data(), code, value);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_kernel_set_led_value()");
    }


    void
    Device::set_clock_id(int clockid)
    {
        int e = ::libevdev_set_clock_id(data(), clockid);
        if (e < 0)
            throw_sys_error(-e, "from libevdev_set_clock_id()");
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
            throw_sys_error(-e, "from libevdev_next_event()");

        return event;
    }


    ReadStatus
    Device::read(Event& event,
                 ReadFlag flags)
        noexcept
    {
        ::input_event raw_event;
        int val = ::libevdev_next_event(data(),
                                        static_cast<unsigned>(flags),
                                        &raw_event);
        if (val >= 0)
            event = raw_event;
        return ReadStatus{val};
    }


    bool
    Device::pending()
    {
        int val = ::libevdev_has_event_pending(data());
        if (val < 0)
            throw_sys_error(-val, "libevdev_has_event_pending()");
        return val;
    }



    // ------------------- //
    // Convenience methods //
    // ------------------- //


    std::vector<Property>
    Device::properties()
        const
    {
        std::vector<Property> result;
        for (unsigned p = 0; p < INPUT_PROP_CNT; ++p)
            if (Property prop{p}; has(prop))
                result.push_back(prop);
        return result;
    }


    std::vector<Type>
    Device::types()
        const
    {
        std::vector<Type> result;
        for (auto t = Type{0}; t <= Type::max(); ++t)
            if (has(t))
                result.push_back(t);
        return result;
    }


    std::vector<Code>
    Device::codes(Type type)
        const
    {
        return codes(type, Code::max(type));
    }


    std::vector<Code>
    Device::codes(Type type,
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


}
