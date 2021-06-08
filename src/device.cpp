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


#include <stdexcept>
#include <string>

#include "device.hpp"

#include "dropped_sync.hpp"
#include "error.hpp"
#include "event.hpp"


using std::logic_error;
using std::runtime_error;
using std::string;
using std::to_string;
using std::uint16_t;

using namespace std::literals;


#define SAFE_ERRNO(call)                        \
    do {                                        \
        if (int e = call; e < 0)                \
            throw Error{-e};                    \
    } while (false)


#define ENFORCE_ZERO(call)                              \
    do {                                                \
        if (call)                                       \
            throw runtime_error{#call + " failed"s};    \
    } while (false)


namespace evdev {


    void
    Device::Deleter::operator()(::libevdev* dev) const noexcept
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
                               va_list args)
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


    Device::Device(LogPriority priority, int filedes) :
        Device{priority}
    {
        fd(filedes);
    }


    Device::Device(LogPriority priority, const path& filename, int flags) :
        Device{priority}
    {
        owned_file.open(filename, flags);
        fd(owned_file.handle());
    }


    Device::Device() :
        dev{::libevdev_new()}
    {
        if (!dev)
            throw runtime_error{"could not construct libevdev device"};

    }


    Device::Device(int f)
    {
        libevdev* ptr = nullptr;
        SAFE_ERRNO(::libevdev_new_from_fd(f, &ptr));
        dev.reset(ptr);
    }


    Device::Device(const path& filename, int flags) :
        owned_file{filename, flags}
    {
        libevdev* ptr = nullptr;
        SAFE_ERRNO(::libevdev_new_from_fd(owned_file.handle(), &ptr));
        dev.reset(ptr);
    }


    Device::~Device() noexcept = default;


    Device::Device(Device&& other) noexcept = default;


    Device&
    Device::operator=(Device&& other) noexcept = default;


    libevdev*
    Device::data() noexcept
    {
        return dev.get();
    }


    const libevdev*
    Device::data() const noexcept
    {
        return dev.get();
    }


    void
    Device::grab()
    {
        SAFE_ERRNO(::libevdev_grab(data(), LIBEVDEV_GRAB));
    }


    void
    Device::ungrab()
    {
        SAFE_ERRNO(::libevdev_grab(data(), LIBEVDEV_UNGRAB));
    }


    void
    Device::fd(int f)
    {
        SAFE_ERRNO(::libevdev_set_fd(data(), f));
    }


    void
    Device::change_fd(int f)
    {
        if (::libevdev_change_fd(data(), f))
            throw runtime_error{__PRETTY_FUNCTION__ + " failed"s};
    }


    int
    Device::fd() const
    {
        int f = ::libevdev_get_fd(data());
        if (f == -1)
            throw logic_error{__PRETTY_FUNCTION__ + " failed"s};
        return f;
    }


    void
    Device::open(const path& filename, int flags)
    {
        owned_file.open(filename, flags);
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


    string
    Device::name() const
    { return ::libevdev_get_name(data()); }


    optional<string>
    Device::phys() const
    {
        auto p = ::libevdev_get_phys(data());
        if (!p)
            return {};
        return {p};
    }


    optional<string>
    Device::uniq() const
    {
        auto u = ::libevdev_get_uniq(data());
        if (!u)
            return {};
        return {u};
    }


    uint16_t
    Device::product_id() const noexcept
    { return ::libevdev_get_id_product(data()); }


    uint16_t
    Device::vendor_id() const noexcept
    { return ::libevdev_get_id_vendor(data()); }


    uint16_t
    Device::bustype() const noexcept
    { return ::libevdev_get_id_bustype(data()); }


    uint16_t
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
            throw runtime_error{"invalid abs info code: "
                    + to_string(Type::abs, code)};
        return *info;
    }


    int
    Device::get(Type type, Code code) const noexcept
    { return ::libevdev_get_event_value(data(), type, code); }


    optional<int>
    Device::fetch(Type type, Code code) const noexcept
    {
        int value;
        // returns a boolean, 0 = error
        if (!::libevdev_fetch_event_value(data(), type, code, &value))
            return {};
        return {value};
    }


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


    optional<int>
    Device::fetch_slot(unsigned slot, Code code) const noexcept
    {
        int value;
        // returns boolean, 0 on error
        if (!::libevdev_fetch_slot_value(data(), slot, code, &value))
            return {};
        return {value};
    }


    optional<int>
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
    Device::name(const string& n)
    { ::libevdev_set_name(data(), n.c_str()); }


    void
    Device::phys(const string& p)
    { ::libevdev_set_phys(data(), p.c_str()); }


    void
    Device::uniq(const string& u)
    { ::libevdev_set_uniq(data(), u.c_str()); }


    void
    Device::product_id(uint16_t pid)
    { ::libevdev_set_id_product(data(), pid); }


    void
    Device::vendor_id(uint16_t vid)
    { ::libevdev_set_id_vendor(data(), vid); }


    void
    Device::bustype(uint16_t bus)
    { ::libevdev_set_id_bustype(data(), bus); }


    void
    Device::version(uint16_t ver)
    { ::libevdev_set_id_version(data(), ver); }


    void
    Device::enable(Property prop)
    {
        ENFORCE_ZERO(::libevdev_enable_property(data(), prop));
    }


    void
    Device::disable(Property prop)
    {
        ENFORCE_ZERO(::libevdev_disable_property(data(), prop));
    }


    void
    Device::set(Type type, Code code, int value)
    {
        ENFORCE_ZERO(::libevdev_set_event_value(data(), type, code, value));
    }


    void
    Device::set_slot(unsigned slot, Code code, int value)
    {
        ENFORCE_ZERO(::libevdev_set_slot_value(data(), slot, code, value));
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
        ENFORCE_ZERO(::libevdev_enable_event_type(data(), type));
    }


    void
    Device::disable(Type type)
    {
        ENFORCE_ZERO(::libevdev_disable_event_type(data(), type));
    }


    void
    Device::enable(Type type,
                   Code code)
    {
        // TODO: use a more descriptive error message
        if (type == Type::abs || type == Type::rep)
            throw logic_error{"wrong overload for this type"};
        ENFORCE_ZERO(::libevdev_enable_event_code(data(), type, code, nullptr));
    }


    void
    Device::enable_abs(Code code,
                       const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        ENFORCE_ZERO(::libevdev_enable_event_code(data(), Type::abs, code, &rinfo));
    }


    void
    Device::enable_rep(Code code,
                       int arg)
    {
        ENFORCE_ZERO(::libevdev_enable_event_code(data(), Type::rep, code, &arg));
    }


    void
    Device::disable(Type type,
                    Code code)
    {
        ENFORCE_ZERO(::libevdev_disable_event_code(data(), type, code));
    }


    void
    Device::kernel_abs_info(Code code, const AbsInfo& info)
    {
        ::input_absinfo rinfo = info;
        SAFE_ERRNO(::libevdev_kernel_set_abs_info(data(), code, &rinfo));
    }


    void
    Device::kernel_led_value(Code code, ::libevdev_led_value value)
    {
        SAFE_ERRNO(::libevdev_kernel_set_led_value(data(), code, value));
    }


    void
    Device::set_clock_id(int clockid)
    {
        SAFE_ERRNO(::libevdev_set_clock_id(data(), clockid));
    }



    // -------------- //
    // Event handling //
    // -------------- //


    Event
    Device::read(Flag flags)
    {
        Event event;
        Status status = read(event, flags);
        if (status == Status::dropped && (flags & Flag::resync) == 0)
            throw DroppedSync{event};

        if (status < 0)
            throw Error{-status};

        return event;
    }


    Status
    Device::read(Event& event, Flag flags) noexcept
    {
        ::input_event raw_event;
        int val = ::libevdev_next_event(data(),
                                        static_cast<unsigned>(flags),
                                        &raw_event);
        if (val >= 0)
            event = raw_event;
        return Status{val};
    }


    bool
    Device::pending()
    {
        int val = ::libevdev_has_event_pending(data());
        SAFE_ERRNO(val);
        return val;
    }



    // ------------------- //
    // Convenience methods //
    // ------------------- //


    vector<Property>
    Device::properties() const
    {
        vector<Property> result;
        for (unsigned p = 0; p < INPUT_PROP_CNT; ++p)
            if (Property prop{p}; has(prop))
                result.push_back(prop);
        return result;
    }


    vector<Type>
    Device::types() const
    {
        vector<Type> result;
        for (uint16_t e = 0; e < EV_CNT; ++e)
            if (Type type{e}; has(type))
                result.push_back(type);
        return result;
    }


    vector<Code>
    Device::codes(Type t) const
    {
        return codes(t, count(t));
    }


    vector<Code>
    Device::codes(Type t, Code count) const
    {
        vector<Code> result;
        for (Code c = 0; c < count; ++c)
            if (has(t, c))
                result.push_back(c);
        return result;
    }


    void
    Device::enable_key(Code code)
    { enable(Type::key, code); }


    void
    Device::enable_rel(Code code)
    { enable(Type::rel, code); }


    void
    Device::enable_abs(Code code)
    { enable(Type::abs, code); }


}
