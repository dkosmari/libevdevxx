/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include <algorithm> // swap()
#include <stdexcept>
#include <string>

#include "libevdevxx/detail/ScopedFile.hpp"

#include "error.hpp"
#include "utils.hpp"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h> // open(), fcntl()
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h> // close()
#endif


using namespace std::literals;


namespace evdev::detail {


    ScopedFile::ScopedFile() noexcept = default;


    ScopedFile::ScopedFile(int fd) noexcept :
        fd{fd}
    {}


    ScopedFile::ScopedFile(const std::filesystem::path& file,
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
    ScopedFile::open(const std::filesystem::path& file,
                     int flags)
    {
        if (is_open())
            throw std::logic_error{"File object is already open."s};

        fd = ::open(file.c_str(), flags);
        if (fd < 0)
            throw_sys_error(errno, "Failed to open \""s + file.string() + "\""s);
    }


    void
    ScopedFile::open(const std::filesystem::path& file,
                     int flags,
                     ::mode_t mode)
    {
        if (is_open())
            throw std::logic_error{"File object is already open."s};

        fd = ::open(file.c_str(), flags, mode);
        if (fd < 0)
            throw_sys_error(errno, "Failed to open \""s + file.string() + "\""s);
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
            throw std::logic_error{"File must be open before it's set to nonblock."};

        int flags = ::fcntl(fd, F_GETFL);
        if (flags < 0)
            throw_sys_error(errno);

        if (enable)
            flags |= O_NONBLOCK;
        else
            flags &= ~O_NONBLOCK;

        if (::fcntl(fd, F_SETFL, flags) < 0)
            throw_sys_error(errno);
    }
}
