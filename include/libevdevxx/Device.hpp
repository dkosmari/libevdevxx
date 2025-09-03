/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBEVDEVXX_DEVICE_HPP
#define LIBEVDEVXX_DEVICE_HPP

#include <cstddef>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <fcntl.h>

#include <libevdev/libevdev.h>

#include "AbsInfo.hpp"
#include "basic_wrapper.hpp"
#include "Event.hpp"
#include "Property.hpp"
#include "ReadFlag.hpp"
#include "ReadStatus.hpp"
#include "TypeCode.hpp"


/// The namespace of libevdevxx.
namespace evdev {

    /// Helper enum to use the logging functions.
    enum class LogPriority {
        none  = -1,
        error = LIBEVDEV_LOG_ERROR,
        info  = LIBEVDEV_LOG_INFO,
        debug = LIBEVDEV_LOG_DEBUG
    };


    /**
     * @brief Represents a device (real of not).
     *
     * In order to use the logging function, the `log()` method must be overridden in a
     * derived class, and a protected constructors (that take a `LogPriority` argument) must
     * be called explicitly.
     */
    class Device :
        public detail::basic_wrapper<libevdev*> {


        using BaseType = detail::basic_wrapper<libevdev*>;

        int owned_fd = -1;

        using state_type = std::tuple<BaseType::state_type, int>;

    protected:


        // ------------------------- //
        // Initialization and setup. //
        // ------------------------- //


        /**
         * @brief Constructor to enable logging.
         *
         * @param priority The priority level; the `log()` method will only be called when
         * the message has this priority or above.
         *
         * @sa `log()`
         */
        Device(LogPriority priority);

        /**
         * @brief Constructor to enable logging, and a non-owning device file descriptor.
         *
         * @param priority The priority level; the `log()` method will only be called when
         * the message has this priority or above.
         *
         * @param fd File descriptor fot the device file (from `/dev/input/event*`) that
         * will be used for I/O. Ownership is not taken over this, so the file will not be
         * closed on destruction.
         *
         * @sa `log()`
         */
        Device(LogPriority priority,
               int fd);

        /**
         * @brief Constructor to enable logging and a device filename.
         *
         * @param priority The priority level; the log() method will only be called when
         * the message has this priority or above.
         *
         * @param filename Path to to an evdev device (from `/dev/input/event*`) that will
         * be open by this device.
         *
         * @param flags Flags used in the `open` system call.
         *
         * @sa log()
         */
        Device(LogPriority priority,
               const std::filesystem::path& filename,
               int flags = O_RDONLY | O_NONBLOCK);


        /**
         * @brief Constructor to enable logging, in-place.
         *
         * @param priority The priority level; the `log()` method will only be called when
         * the message has this priority or above.
         *
         * @sa `log()`
         */
        void
        create(LogPriority priority);


        /**
         * @brief Constructor to enable logging, and a non-owning device file descriptor,
         * in-place.
         *
         * @param priority The priority level; the `log()` method will only be called when
         * the message has this priority or above.
         *
         * @param fd File descriptor fot the device file (from `/dev/input/event*`) that
         * will be used for I/O. Ownership is not taken over this, so the file will not be
         * closed on destruction.
         *
         * @sa `log()`
         */
        void
        create(LogPriority priority,
               int fd);


        /**
         * @brief Constructor to enable logging and a device filename, in-place.
         *
         * @param priority The priority level; the log() method will only be called when
         * the message has this priority or above.
         *
         * @param filename Path to to an evdev device (from `/dev/input/event*`) that will
         * be open by this device.
         *
         * @param flags Flags used in the `open` system call.
         *
         * @sa log()
         */
        void
        create(LogPriority priority,
               const std::filesystem::path& filename,
               int flags = O_RDONLY | O_NONBLOCK);


    public:

        /**
         * @brief Construct invalid device.
         *
         * In an invalid state, most operations are not allowed. You can safely destroy,
         * move and check for validity.
         */
        Device(std::nullptr_t)
            noexcept;

        /**
         * @brief Default constructor.
         *
         * No device file is associated with this device. For this to have any use, the
         * user must later do either:
         *
         *   - call `open()` to open a device file.
         *   - call `set_fd()` to supply a device file descriptor.
         *   - pass it to a `Uinput` object, that will turn it into a virtual device.
         *
         * @sa `open()`, `set_fd(int)`, `Uinput`
         */
        Device();

        /**
         * @brief Construct form a device file descriptor.
         *
         * The file descriptor is not owned: it won't be closed during destruction.
         *
         * @sa `set_fd(int)`
         */
        Device(int fd);

        /**
         * @brief Construct from a device file path.
         */
        Device(const std::filesystem::path& filename,
               int flags = O_RDONLY | O_NONBLOCK);


        ~Device()
            noexcept;


        /**
         * @brief Create a new instance, in-place.
         *
         * No device file is associated with this device. For this to have any use, the
         * user must later do either:
         *
         *   - call `open()` to open a device file.
         *   - call `set_fd()` to supply a device file descriptor.
         *   - pass it to a `Uinput` object, that will turn it into a virtual device.
         *
         * @sa `open()`, `set_fd(int)`, `Uinput`
         */
        void
        create();


        /**
         * @brief Construct form a device file descriptor, in-place.
         *
         * The file descriptor is not owned: it won't be closed during destruction.
         *
         * @sa `set_fd(int)`
         */
        void
        create(int fd);


        /**
         * @brief Construct from a device file path, in-place.
         */
        void
        create(const std::filesystem::path& filename,
               int flags = O_RDONLY | O_NONBLOCK);


        /**
         * @brief free all allocated resources and make the instance invalid.
         *
         * The result is the same as if this object was moved from.
         */
        void
        destroy()
            noexcept override;


        /// Move constructor.
        Device(Device&& other)
            noexcept;


        /// Move assignment.
        Device&
        operator =(Device&& other)
            noexcept;


        void
        acquire(libevdev* new_raw,
                int new_fd)
            noexcept;


        void
        acquire(state_type new_state)
            noexcept;


        state_type
        release()
            noexcept;


        /**
         * @brief Grab the device through a `EVIOCGRAB` syscall.
         *
         * Use `Grabber` to grab and ungrab with exception-safety.
         *
         * @throw std::system_error
         *
         * @sa Grabber
         */
        void
        grab();

        /**
         * @brief Ungrab the device.
         *
         * @throw std::system_error
         *
         * @sa Grabber
         */
        void
        ungrab();

        /**
         * @brief Set a file descriptor and read the device metadata.
         *
         * You can only call this function once.
         */
        void
        set_fd(int fd);

        /**
         * @brief Change the file descriptor used internally, without re-reading the
         * actual device.
         *
         * This is only useful if you need to close and reopen the file descriptor.
         */
        void
        change_fd(int fd);

        /// Return the internal file descriptor used to access the device file.
        int
        get_fd()
            const;

        void
        open(const std::filesystem::path& filename,
                 int flags = O_RDONLY | O_NONBLOCK);

        // only valid if Device opened the file, not if given a fd.
        bool
        is_open()
            const noexcept;

        void
        close()
            noexcept;

        void
        set_nonblock(bool enable);


        // ------------------ //
        // Logging facilities //
        // ------------------ //

        virtual
        void
        log(LogPriority p,
            const char* file,
            int line,
            const char* func,
            const char* format,
            std::va_list args)
            const noexcept;


        // --------------------- //
        // Querying capabilities //
        // --------------------- //


        std::string
        get_name()
            const;

        std::optional<std::string>
        get_phys()
            const;

        std::optional<std::string>
        get_uniq()
            const;

        std::uint16_t
        get_product()
            const noexcept;

        std::uint16_t
        get_vendor()
            const noexcept;

        std::uint16_t
        get_bustype()
            const noexcept;

        std::uint16_t
        get_version()
            const noexcept;

        int
        get_driver_version()
            const noexcept;


        bool
        has(Property prop)
            const noexcept;

        bool
        has(Type type)
            const noexcept;

        bool
        has(Type type, Code code)
            const noexcept;

        bool
        has(const TypeCode& tc)
            const noexcept;


        int
        get_abs_min (Code code)
            const noexcept;
        int
        get_abs_max (Code code)
            const noexcept;
        int
        get_abs_fuzz(Code code)
            const noexcept;
        int
        get_abs_flat(Code code)
            const noexcept;
        int
        get_abs_res (Code code)
        const noexcept;

        AbsInfo
        get_abs_info(Code code)
            const;

        int
        get_value(Type type,
                  Code code)
            const noexcept;

        int
        get_value(const TypeCode& tc)
            const noexcept;

        std::optional<int>
        try_get_value(Type type,
                      Code code)
            const noexcept;

        std::optional<int>
        try_get_value(const TypeCode& tc)
            const noexcept;

        bool
        get_repeat(int& delay,
                   int& period)
            const noexcept;


        struct DelayPeriod {
            int delay;
            int period;
        };

        std::optional<DelayPeriod>
        try_get_repeat()
            const noexcept;


        // ----------------------------- //
        // Multi-touch related functions //
        // ----------------------------- //


        int
        get_slot(unsigned slot,
                 Code code)
            const noexcept;

        std::optional<int>
        try_get_slot(unsigned slot,
                     Code code)
            const noexcept;

        int
        get_num_slots()
            const;

        std::optional<int>
        try_get_num_slots()
            const noexcept;

        int
        get_current_slot()
            const noexcept;


        // ---------------------------------------- //
        // Modifying the appearance or capabilities //
        // ---------------------------------------- //


        void
        set_name(const std::string& name)
            noexcept;

        void
        set_phys(const std::string& phys)
            noexcept;

        void
        set_uniq(const std::string& uniq)
            noexcept;

        void
        set_product(std::uint16_t pid)
            noexcept;

        void
        set_vendor(std::uint16_t vid)
            noexcept;

        void
        set_bustype(std::uint16_t bus)
            noexcept;

        void
        set_version(std::uint16_t ver)
            noexcept;

        void
        enable(Property prop);

        void
        disable(Property prop);

        void
        set_value(Type type,
                  Code code,
                  int value);

        void
        set_value(const TypeCode& tc,
                  int value);

        void
        set_slot(unsigned slot,
                 Code code,
                 int value);

        void
        set_abs_min(Code code,
                    int val)
            noexcept;

        void
        set_abs_max(Code code,
                    int val)
            noexcept;

        void
        set_abs_fuzz(Code code,
                     int val)
            noexcept;

        void
        set_abs_flat(Code code,
                     int val)
            noexcept;

        void
        set_abs_res(Code code,
                    int val)
            noexcept;

        void
        set_abs_info(Code code,
                     const AbsInfo& abs)
            noexcept;

        void
        enable(Type type);

        void
        disable(Type type);

        void
        enable(Type type,
               Code code);

        void
        enable(const TypeCode& tc);

        void
        enable_abs(Code code,
                   const AbsInfo& info);

        void
        enable_rep(Code code,
                   int arg);

        void
        disable(Type type,
                Code code);

        void
        disable(const TypeCode& tc);

        void
        set_kernel_abs_info(Code code,
                            const AbsInfo& abs);

        void
        set_kernel_led_value(Code code,
                             libevdev_led_value value);

        void
        set_clock_id(int clockid);


        // -------------- //
        // Event handling //
        // -------------- //


        Event
        read(ReadFlag flags = ReadFlag::normal);

        ReadStatus
        read(Event& event,
             ReadFlag flags = ReadFlag::normal)
            noexcept;

        bool
        has_pending();


        // ------------------- //
        // Convenience methods //
        // ------------------- //


        std::vector<Property>
        get_properties()
            const;

        std::vector<Type>
        get_types()
            const;

        std::vector<Code>
        get_codes(Type type)
            const;

        std::vector<Code>
        get_codes(Type type,
                  Code max)
            const;


        void
        enable_key(Code code);

        void
        enable_rel(Code code);

        void
        enable_abs(Code code);

    }; // class Device

} // namespace evdev

#endif
