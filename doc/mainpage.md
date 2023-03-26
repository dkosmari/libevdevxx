Introduction
============

[libevdevxx](http://github.com/dkosmari/libevdevxx) is a C++20 wrapper for the C library
[libevdev](http://www.freedesktop.org/wiki/Software/libevdev), which is a high-level C
library for the `evdev` Linux driver. This library exposes all of @e libevdev as C++
classes and methods, with RAII and type safety.

It's released under the [MIT license](http://spdx.org/licenses/MIT.html), the same used by
libevdev.


Example
-------

In this example, a virtual device is created to mimick a mouse. Then events are inserted
to simulate the mouse moving in a circle.

@include circle-mouse.cpp

