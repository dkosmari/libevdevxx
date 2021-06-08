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


#ifndef LIBEVDEVXX_SCOPED_FILE_HPP
#define LIBEVDEVXX_SCOPED_FILE_HPP


#include <filesystem>

#include <fcntl.h> // mode_t


namespace evdev::internal {

    using std::filesystem::path;


    class ScopedFile {

        int fd = -1;

    public:


        ScopedFile() noexcept;
        ScopedFile(int fd) noexcept;
        ScopedFile(const path& file, int flags);

        ~ScopedFile() noexcept;

        // move operations
        ScopedFile(ScopedFile&& other) noexcept;
        ScopedFile& operator=(ScopedFile&& other) noexcept;


        int handle() const noexcept;
        void handle(int f);


        void open(const path& file,
                  int flags);
        void open(const path& file,
                  int flags,
                  ::mode_t mode);

        void close() noexcept;

        bool is_open() const noexcept;


        void nonblock(bool enable);

    };

}


#endif
