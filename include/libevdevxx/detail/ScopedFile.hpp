/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_SCOPED_FILE_HPP
#define LIBEVDEVXX_SCOPED_FILE_HPP


#include <filesystem>

#include <fcntl.h> // mode_t


namespace evdev::detail {


    class ScopedFile {

        int fd = -1;

    public:


        ScopedFile() noexcept;

        ScopedFile(int fd) noexcept;

        ScopedFile(const std::filesystem::path& file,
                    int flags);

        ~ScopedFile() noexcept;


        // move operations
        ScopedFile(ScopedFile&& other) noexcept;
        ScopedFile& operator =(ScopedFile&& other) noexcept;


        int handle() const noexcept;
        void handle(int f);


        void open(const std::filesystem::path& file,
                  int flags);

        void open(const std::filesystem::path& file,
                  int flags,
                  ::mode_t mode);

        void close() noexcept;

        bool is_open() const noexcept;


        void nonblock(bool enable);

    };

} // namespace evdev::detail


#endif
