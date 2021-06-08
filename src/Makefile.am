# src/Makefile.am

AM_CXXFLAGS = \
	-Wall -Wextra -Werror


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS)




lib_LTLIBRARIES = libevdevxx.la


libevdevxxdir = $(includedir)/libevdevxx

libevdevxx_HEADERS = \
	absinfo.hpp \
	device.hpp \
	dropped_sync.hpp \
	error.hpp \
	event.hpp \
	grabber.hpp \
	property.hpp \
	read.hpp \
	scoped_file.hpp \
	uinput.hpp


libevdevxx_la_SOURCES = \
	absinfo.cpp \
	device.cpp \
	dropped_sync.cpp \
	error.cpp \
	event.cpp \
	grabber.cpp \
	private_utils.hpp private_utils.cpp \
	property.cpp \
	read.cpp \
	scoped_file.cpp \
	uinput.cpp


libevdevxx_la_LIBADD = \
	$(LIBEVDEV_LIBS)


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
