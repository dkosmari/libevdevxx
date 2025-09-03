# libevdevxx - a C++ wrapper for libevdev


`libevdevxx` is a C++20 wrapper for the C library `libevdev`, which is a high-level C
library for the `evdev` Linux driver. This library exposes all of `libevdev` as C++ classes
and methods, with RAII and type safety.


## Example

This example ([examples/circle-mouse.cpp](examples/circle-mouse.cpp)) creates a virtual
mouse, through the `uinput` subsystem, and generates events to simulate the mouse moving
in a circle.

```cpp
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include <libevdevxx/evdevxx.hpp>


using std::cout;
using std::endl;
using std::flush;

using namespace std::literals;


int
main()
{
    using evdev::Code;

    evdev::Device dev;

    dev.set_name("Fake Mouse");

    dev.enable_rel(Code{REL_X});
    dev.enable_rel(Code{REL_Y});
    // needs at least one button to be recognized as a mouse
    dev.enable_key(Code{BTN_LEFT});

    evdev::Uinput udev{dev};
    cout << "Created uinput device at "
         << udev.devnode().value()
         << endl;
    cout << "Doing a circle... " << flush;

    const float radius = 10;
    for (int i = 0; i < 100; ++i) {

        float angle = i/100.0f * 2 * M_PI;
        int x = static_cast<int>(radius * std::cos(angle));
        int y = static_cast<int>(radius * std::sin(angle));

        udev.rel(Code{REL_X}, x);
        udev.rel(Code{REL_Y}, y);
        udev.flush();

        std::this_thread::sleep_for(16ms);
    }

    cout << "done." << endl;
}
```

See the contents of [examples](examples) and [tools](tools) for more examples.


## Building

### Dependencies

- A C++20 compiler, usually installed by a meta package like `task-c++` or `build-essential`.

- C/C++ compilation tools such as:
  - `autoconf`
  - `automake`
  - `libtool`
  - `pkg-config`

- [libevdev](http://www.freedesktop.org/wiki/Software/libevdev) 1.13+: usually available as a
  package in your distro (you need the "devel" package.)


### Instructions

You can obtain the source through a release tarball, or by cloning the repository. If you
use a release tarball, you can skip step 0.

0. `./bootstrap`
1. `./configure`
2. `make`
3. `sudo make install`

This is a standard Automake package; more installation details can be found in the file
[INSTALL](INSTALL) or by running `./configure --help`.
