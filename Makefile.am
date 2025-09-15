# ./Makefile.am

ACLOCAL_AMFLAGS = -I m4


EXTRA_DIST = \
	bootstrap \
	libevdevxx.pc.in \
	README.md


SUBDIRS = \
	. \
	examples \
	tools \
	doc


libevdevxxdir = $(includedir)/libevdevxx

libevdevxx_HEADERS = \
	include/libevdevxx/AbsInfo.hpp \
	include/libevdevxx/basic_wrapper.hpp \
	include/libevdevxx/Code.hpp \
	include/libevdevxx/Device.hpp \
	include/libevdevxx/evdevxx.hpp \
	include/libevdevxx/Event.hpp \
	include/libevdevxx/Grabber.hpp \
	include/libevdevxx/NumberBase.hpp \
	include/libevdevxx/Property.hpp \
	include/libevdevxx/ReadFlag.hpp \
	include/libevdevxx/ReadStatus.hpp \
	include/libevdevxx/SyncError.hpp \
	include/libevdevxx/Type.hpp \
	include/libevdevxx/TypeCode.hpp \
	include/libevdevxx/Uinput.hpp



AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	-I$(srcdir)/include \
	$(LIBEVDEV_CFLAGS)


lib_LTLIBRARIES = libevdevxx.la


libevdevxx_la_SOURCES = \
	src/AbsInfo.cpp \
	src/Code.cpp \
	src/Device.cpp \
	src/error.cpp \
	src/error.hpp \
	src/Event.cpp \
	src/Grabber.cpp \
	src/Property.cpp \
	src/ReadFlag.cpp \
	src/ReadStatus.cpp \
	src/SyncError.cpp \
	src/Type.cpp \
	src/TypeCode.cpp \
	src/Uinput.cpp \
	src/utils.hpp


libevdevxx_la_LIBADD = $(LIBEVDEV_LIBS)


pcfiledir = $(pkgconfigdir)
pcfile_DATA = libevdevxx.pc


# Don't leave empty directories behind during uninstall
uninstall-hook:
	-rmdir --ignore-fail-on-non-empty $(DESTDIR)$(libevdevxxdir)


.PHONY: company docs


company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt


docs:
	$(MAKE) --directory doc
