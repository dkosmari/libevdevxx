/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2021-2023  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */


#ifndef LIBEVDEVXX_SYNC_ERROR_HPP
#define LIBEVDEVXX_SYNC_ERROR_HPP


#include <stdexcept>

#include "Event.hpp"


namespace evdev {


    struct SyncError : std::runtime_error {

        Event event;

        SyncError(const Event& event);

    };


} // namespace evdev


#endif
