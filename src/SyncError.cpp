/*
 * libevdevxx - a C++ wrapper for libevdev
 *
 * Copyright (C) 2025  Daniel K. O.
 * SPDX-License-Identifier: MIT
 */

#include "libevdevxx/SyncError.hpp"


namespace evdev {

    SyncError::SyncError(const Event& event) :
        std::runtime_error{"sync error"},
        event{event}
    {}

} // namespace evdev
