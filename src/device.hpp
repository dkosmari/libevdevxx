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

#include "abs_info.hpp"
#include "event.hpp"
#include "property.hpp"
#include "read.hpp"
#include "scoped_file.hpp"


namespace evdev {


    using std::string;
    using std::optional;
    using std::uint16_t;
    using std::filesystem::path;
    using std::vector;

    using opt_string = optional<string>;
    using opt_int = optional<int>;




    enum class LogPriority {
        none  = -1,
        error = LIBEVDEV_LOG_ERROR,
        info  = LIBEVDEV_LOG_INFO,
        debug = LIBEVDEV_LOG_DEBUG
    };


    class Device {

        struct Deleter {
            void operator()(::libevdev* dev) const noexcept;
        };

        std::unique_ptr<::libevdev, Deleter> dev;

        internal::ScopedFile owned_file;


        // ------------------------- //
        // Initialization and setup. //
        // ------------------------- //

    protected:
        Device(LogPriority priority);
        Device(LogPriority priority, int filedes);
        Device(LogPriority priority, const path& filename, int flags = O_RDONLY);

    public:

        Device();
        Device(int f);
        Device(const path& filename, int flags = O_RDONLY | O_NONBLOCK);


        virtual ~Device() noexcept;


        // move operations
        Device(Device&& other) noexcept;
        Device& operator=(Device&& other) noexcept;



        ::libevdev* data() noexcept;
        const ::libevdev* data() const noexcept;


        void grab();
        void ungrab();


        // supply a file descriptor without owning it
        void fd (int fd);
        void change_fd(int fd);
        int fd() const;


        void open(const path& filename, int flags = O_RDONLY | O_NONBLOCK);
        // only valid if Device opened the file, not if given a fd.
        bool is_open() const noexcept;
        void close() noexcept;
        void nonblock(bool enable);


        // ------------------ //
        // Logging facilities //
        // ------------------ //

        virtual
        void log(LogPriority p,
                 const char* file, int line, const char* func,
                 const char* format, va_list args) const;



        // --------------------- //
        // Querying capabilities //
        // --------------------- //

        string name() const;
        opt_string phys() const;
        opt_string uniq() const;

        uint16_t product_id() const noexcept;
        uint16_t vendor_id() const noexcept;
        uint16_t bustype() const noexcept;
        uint16_t version() const noexcept;

        int driver_version() const noexcept;


        bool has(Property prop) const noexcept;


        bool has(Type type) const noexcept;
        bool has(Type type, Code code) const noexcept;

        int abs_min (Code code) const noexcept;
        int abs_max (Code code) const noexcept;
        int abs_fuzz(Code code) const noexcept;
        int abs_flat(Code code) const noexcept;
        int abs_res (Code code) const noexcept;

        AbsInfo abs_info(Code code) const;

        int get(Type type, Code code) const noexcept;
        opt_int fetch(Type type, Code code) const noexcept;

        bool get_repeat(int& delay, int& period) const noexcept;



        // ----------------------------- //
        // Multi-touch related functions //
        // ----------------------------- //

        // TODO: these codes should be enums

        int get_slot(unsigned slot, Code code) const noexcept;
        opt_int fetch_slot(unsigned slot, Code code) const noexcept;
        opt_int num_slots() const noexcept;
        int current_slot() const noexcept;



        // ---------------------------------------- //
        // Modifying the appearance or capabilities //
        // ---------------------------------------- //

        void name(const string& n);
        void phys(const string& phys);
        void uniq(const string& uniq);

        void product_id(uint16_t pid);
        void vendor_id(uint16_t vid);
        void bustype(uint16_t bus);
        void version(uint16_t ver);

        void enable(Property prop);
        void disable(Property prop);

        void set(Type type, Code code, int value);

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
        void enable_abs(Code code, const AbsInfo& info);
        void enable_rep(Code code, int arg);
        void disable(Type type, Code code);

        void kernel_abs_info(Code code, const AbsInfo& abs);
        void kernel_led_value(Code code, ::libevdev_led_value value);

        void set_clock_id(int clockid);



        // -------------- //
        // Event handling //
        // -------------- //

        Event read(Flag flags = Flag::normal);
        Status read(Event& event, Flag flags = Flag::normal) noexcept;
        bool pending();


        // ------------------- //
        // Convenience methods //
        // ------------------- //

        vector<Property> properties() const;

        vector<Type> types() const;

        vector<Code> codes(Type t) const;
        vector<Code> codes(Type t, Code count) const;


        void enable_key(Code code);
        void enable_rel(Code code);
        void enable_abs(Code code);
    };


} // namespace evdev


#endif
