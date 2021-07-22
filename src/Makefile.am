# src/Makefile.am


AM_CXXFLAGS = -Wall -Wextra -Werror


AM_CPPFLAGS = $(LIBEVDEV_CFLAGS)



libevdevxxdir = $(includedir)/libevdevxx
libevdevxx_HEADERS = \
	abs_info.hpp \
	device.hpp \
	error.hpp \
	evdevxx.hpp \
	event.hpp \
	grabber.hpp \
	number_base.hpp \
	property.hpp \
	read.hpp \
	scoped_file.hpp \
	sync_error.hpp \
	uinput.hpp


lib_LTLIBRARIES = libevdevxx.la


libevdevxx_la_SOURCES = \
	abs_info.cpp \
	device.cpp \
	error.cpp \
	event.cpp \
	event_code.cpp \
	event_type.cpp \
	grabber.cpp \
	private_utils.hpp private_utils.cpp \
	property.cpp \
	read.cpp \
	scoped_file.cpp \
	sync_error.cpp \
	uinput.cpp


libevdevxx_la_LIBADD = $(LIBEVDEV_LIBS)


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
