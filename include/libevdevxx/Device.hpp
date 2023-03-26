/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_DEVICE_HPP
#define LIBEVDEVXX_DEVICE_HPP


#include <cstdarg>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <libevdev/libevdev.h>

#include "AbsInfo.hpp"
#include "Event.hpp"
#include "Property.hpp"
#include "ReadFlag.hpp"
#include "ReadStatus.hpp"
#include "TypeCode.hpp"

#include "detail/ScopedFile.hpp"

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
    class Device {

        struct Deleter {
            void operator()(::libevdev* dev) const noexcept;
        };

        std::unique_ptr<::libevdev, Deleter> dev;

        detail::ScopedFile owned_file;


        // ------------------------- //
        // Initialization and setup. //
        // ------------------------- //

    protected:

        /**
         * @brief Constructor to enable logging.
         *
         * @param priority The priority level; the log() method will only be called when
         * the message has this priority or above.
         *
         * @sa log()
         */
        Device(LogPriority priority);

        /**
         * @brief Constructor to enable logging, and a non-owning device file descriptor.
         *
         * @param priority The priority level; the log() method will only be called when
         * the message has this priority or above.
         *
         * @param filedes File descriptor fot the device file (from `/dev/input/event*`)
         * that will be used for I/O. Ownership is not taken over this, so the file will
         * not be closed on destruction.
         *
         * @sa log()
         */
        Device(LogPriority priority,
               int filedes);

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


    public:

        /**
         * @brief Default constructor.
         *
         * No device file is associated with this device. For this to have any use, the
         * user must later do either:
         *
         *   - call open() to open a device file.
         *   - call fd() to supply a device file descriptor.
         *   - pass it to a `Uinput` object, that will turn it into a virtual device.
         *
         * @sa open(), fd(int), Uinput
         */
        Device();

        /**
         * @brief Construct form a device file descriptor. The file descriptor is not owned.
         *
         * @sa fd(int)
         */
        Device(int filedes);

        /**
         * @brief Construct from a device file path.
         */
        Device(const std::filesystem::path& filename,
               int flags = O_RDONLY | O_NONBLOCK);

        virtual ~Device() noexcept;


        /// Access the internal `::libevdev` object.
        ::libevdev* data() noexcept;

        /// Access the internal `::libevdev` object, const version.
        const ::libevdev* data() const noexcept;


        /**
         * @brief Grab the device through a `EVIOCGRAB` syscall.
         *
         * Use `Grabber` to grab and ungrab with exception-safety.
         *
         * @throw std::system_error
         *
         * @sa Grabber
         */
        void grab();

        /**
         * @brief Ungrab the device.
         *
         * @throw std::system_error
         *
         * @sa Grabber
         */
        void ungrab();


        /**
         * @brief Set a file descriptor and read the device metadata.
         *
         * You can only call this function once.
         */
        void fd (int fd);

        /**
         * @brief Change the file descriptor used internally, without re-reading the actual
         * device.
         *
         * This is only useful if you need to close and reopen the file descriptor.
         */
        void change_fd(int fd);

        /// Return the internal file descriptor used to access the device file.
        int fd() const;


        void open(const std::filesystem::path& filename,
                  int flags = O_RDONLY | O_NONBLOCK);
        // only valid if Device opened the file, not if given a fd.
        bool is_open() const noexcept;
        void close() noexcept;
        void nonblock(bool enable);


        // ------------------ //
        // Logging facilities //
        // ------------------ //

        virtual
        void
        log(LogPriority p,
            const char* file, int line, const char* func,
            const char* format, std::va_list args)
            const;



        // --------------------- //
        // Querying capabilities //
        // --------------------- //

        std::string name() const;
        std::optional<std::string> phys() const;
        std::optional<std::string> uniq() const;

        std::uint16_t product() const noexcept;
        std::uint16_t vendor() const noexcept;
        std::uint16_t bustype() const noexcept;
        std::uint16_t version() const noexcept;

        int driver_version() const noexcept;


        bool has(Property prop) const noexcept;


        bool has(Type type) const noexcept;
        bool has(Type type, Code code) const noexcept;
        bool has(const TypeCode& tc) const noexcept;

        int abs_min (Code code) const noexcept;
        int abs_max (Code code) const noexcept;
        int abs_fuzz(Code code) const noexcept;
        int abs_flat(Code code) const noexcept;
        int abs_res (Code code) const noexcept;

        AbsInfo abs_info(Code code) const;

        int get(Type type, Code code) const noexcept;
        int get(const TypeCode& tc) const noexcept;

        std::optional<int> fetch(Type type, Code code) const noexcept;
        std::optional<int> fetch(const TypeCode& tc) const noexcept;

        bool get_repeat(int& delay, int& period) const noexcept;



        // ----------------------------- //
        // Multi-touch related functions //
        // ----------------------------- //

        int
        get_slot(unsigned slot,
                 Code code)
            const noexcept;

        std::optional<int>
        fetch_slot(unsigned slot,
                   Code code)
            const noexcept;

        std::optional<int>
        num_slots()
            const noexcept;

        int
        current_slot()
            const noexcept;



        // ---------------------------------------- //
        // Modifying the appearance or capabilities //
        // ---------------------------------------- //

        void name(const std::string& n);
        void phys(const std::string& phys);
        void uniq(const std::string& uniq);

        void product (std::uint16_t pid);
        void vendor  (std::uint16_t vid);
        void bustype (std::uint16_t bus);
        void version (std::uint16_t ver);

        void enable(Property prop);
        void disable(Property prop);

        void set(Type type, Code code, int value);
        void set(const TypeCode& tc, int value);

        void set_slot(unsigned slot, Code code, int value);

        void abs_min (Code code, int val);
        void abs_max (Code code, int val);
        void abs_fuzz(Code code, int val);
        void abs_flat(Code code, int val);
        void abs_res (Code code, int val);

        void abs_info(Code code, const AbsInfo& abs);

        void enable(Type type);
        void disable(Type type);

        void enable(Type type, Code code);
        void enable(const TypeCode& tc);

        void enable_abs(Code code, const AbsInfo& info);
        void enable_rep(Code code, int arg);

        void disable(Type type, Code code);
        void disable(const TypeCode& tc);

        void kernel_abs_info(Code code, const AbsInfo& abs);
        void kernel_led_value(Code code, ::libevdev_led_value value);

        void set_clock_id(int clockid);



        // -------------- //
        // Event handling //
        // -------------- //

        Event
        read(ReadFlag flags = ReadFlag::normal);

        ReadStatus
        read(Event& event,
             ReadFlag flags = ReadFlag::normal)
            noexcept;

        bool pending();


        // ------------------- //
        // Convenience methods //
        // ------------------- //

        std::vector<Property>
        properties()
            const;

        std::vector<Type>
        types()
            const;

        std::vector<Code>
        codes(Type type)
            const;

        std::vector<Code>
        codes(Type type,
              Code max)
            const;


        void enable_key(Code code);
        void enable_rel(Code code);
        void enable_abs(Code code);


    }; // class Device


} // namespace evdev


#endif
