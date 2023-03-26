Install
=======

Here are some installation instructions.


Prerequisites
-------------

Before you can build libevdevxx, make sure you have:

  - A C++20 compiler.
  
  - [libevdev](http://www.freedesktop.org/wiki/Software/libevdev) version 1.10 or above.
  
  - Optional: [Doxygen](http://doxygen.nl/) (for building the documentation)


Getting the source
------------------

### From a tarball ###

If you downloaded the tarball, simply extract it with a command like:

    tar xvf libevdevxx-XYZ.tar.gz

where `XYZ` is the version.

### From the repository ###

When obtaining the source code from the repository, you will need to generate the
`configure` script, by running the `bootstrap` script:

    ./bootstrap


Building
--------

Since libevdevxx uses Automake, the usual Automake steps apply:

    ./configure
    make
    sudo make install

For more details, see the file [INSTALL](../INSTALL) or run `./configure --help`.
