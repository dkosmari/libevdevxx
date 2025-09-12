#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([libevdevxx],
        [0.5.1],
        [https://github.com/dkosmari/libevdevxx/issues],
        [],
        [https://github.com/dkosmari/libevdevxx])
AC_CONFIG_SRCDIR([src/AbsInfo.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects])
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
AX_APPEND_COMPILE_FLAGS([-std=c++20], [CXX])


ENABLE_DOCS=no
AC_ARG_ENABLE([docs],
              [AS_HELP_STRING([--enable-docs], [enable generating documentation])],
              [ENABLE_DOCS=$enableval])
AS_VAR_IF([ENABLE_DOCS], [yes],
          [
              AC_CHECK_PROGS([DOXYGEN], [doxygen], [:])
              AS_VAR_IF([DOXYGEN], [:], [ENABLE_DOCS=no])
          ])
AM_CONDITIONAL([BUILD_DOCS], [test "x$ENABLE_DOCS" = "xyes"])


ENABLE_TOOLS=no
AC_ARG_ENABLE([tools],
              [AS_HELP_STRING([--enable-tools], [enable building tools])],
              [ENABLE_TOOLS=$enableval])
AM_CONDITIONAL([BUILD_TOOLS], [test "x$ENABLE_TOOLS" = "xyes"])


ENABLE_EXAMPLES=no
AC_ARG_ENABLE([examples],
              [AS_HELP_STRING([--enable-examples], [enable building examples])],
              [ENABLE_EXAMPLES=$enableval])
AM_CONDITIONAL([BUILD_EXAMPLES], [test "x$ENABLE_EXAMPLES" = "xyes"])



PKG_INSTALLDIR


PKG_CHECK_MODULES([LIBEVDEV], [libevdev >= 1.10])


AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([sys/ioctl.h])
AC_CHECK_HEADERS([unistd.h])


AC_TYPE_MODE_T


AC_FUNC_STRERROR_R


TARBALL_NAME="${PACKAGE_TARNAME}-${PACKAGE_VERSION}.tar.gz"
AC_SUBST([TARBALL_NAME])

AC_CONFIG_FILES([Makefile
                doc/Doxyfile
                doc/Makefile
                examples/Makefile
                libevdevxx.pc
                libevdevxx.spec
                tools/Makefile
])
AC_OUTPUT

AC_MSG_NOTICE([Building documentation: $ENABLE_DOCS])
AC_MSG_NOTICE([Building tools: $ENABLE_TOOLS])
AC_MSG_NOTICE([Building examples: $ENABLE_EXAMPLES])
