#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([libevdevxx], [0.2], [])
AC_CONFIG_SRCDIR([src/device.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE
LT_INIT

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17([noext])

PKG_INSTALLDIR

# Checks for libraries.
PKG_CHECK_MODULES([LIBEVDEV], [libevdev >= 1.6])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([sys/ioctl.h])
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_MODE_T

# Checks for library functions.
AC_FUNC_STRERROR_R

AC_CONFIG_FILES([
examples/Makefile
libevdevxx.pc
libevdevxx.spec
Makefile
src/Makefile
tools/Makefile
])
AC_OUTPUT
