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


#include <algorithm> // swap()
#include <stdexcept>
#include <string>

#include "scoped_file.hpp"

#include "error.hpp"
#include "private_utils.hpp"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h> // open(), fcntl()
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h> // close()
#endif


using std::runtime_error;
using std::filesystem::path;
using std::string;

using namespace std::literals;

using evdev::priv::list_to_string;



namespace evdev::internal {


    ScopedFile::ScopedFile() noexcept = default;


    ScopedFile::ScopedFile(int fd) noexcept :
        fd{fd}
    {}


    ScopedFile::ScopedFile(const path& file,
                           int flags)
    {
        open(file, flags);
    }


    ScopedFile::~ScopedFile() noexcept
    {
        close();
    }


    ScopedFile::ScopedFile(ScopedFile&& other) noexcept :
        ScopedFile{other.fd}
    {
        other.fd = -1;
    }


    ScopedFile&
    ScopedFile::operator=(ScopedFile&& other) noexcept
    {
        if (this != &other) {
            close();
            std::swap(fd, other.fd);
        }
        return *this;
    }


    int
    ScopedFile::handle() const noexcept
    {
        return fd;
    }


    void
    ScopedFile::handle(int f)
    {
        close();
        fd = f;
    }


    void
    ScopedFile::open(const path& file,
                     int flags)
    {
        if (is_open())
            throw runtime_error{"file object is already open"s};

        fd = ::open(file.c_str(), flags);
        if (fd < 0)
            throw Error{"::open("s
                    + list_to_string(file, flags)
                    + ") failed"s, errno};
    }


    void
    ScopedFile::open(const path& file,
                     int flags,
                     ::mode_t mode)
    {
        if (is_open())
            throw runtime_error{"file object is already open"s};

        fd = ::open(file.c_str(), flags, mode);
        if (fd < 0)
            throw Error{"::open("s
                    + list_to_string(file, flags, mode)
                    + ") failed"s, errno};

    }


    void
    ScopedFile::close() noexcept
    {
        if (is_open()) {
            ::close(fd);
            fd = -1;
        }
    }


    bool
    ScopedFile::is_open() const noexcept
    {
        return fd >= 0;
    }


    void
    ScopedFile::nonblock(bool enable)
    {
        if (!is_open())
            throw Error{EBADFD};

        int flags = ::fcntl(fd, F_GETFL);
        if (flags < 0)
            throw Error{errno};

        if (enable)
            flags |= O_NONBLOCK;
        else
            flags &= ~O_NONBLOCK;

        if (::fcntl(fd, F_SETFL, flags) < 0)
            throw Error{errno};
    }
}
