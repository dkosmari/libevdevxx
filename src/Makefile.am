# src/Makefile.am


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(LIBEVDEV_CFLAGS)


lib_LTLIBRARIES = libevdevxx.la


libevdevxx_la_SOURCES = \
	AbsInfo.cpp \
	Type.cpp \
	Code.cpp \
	error.cpp error.hpp \
	Property.cpp \
	ReadFlag.cpp \
	ReadStatus.cpp \
	Event.cpp \
	Device.cpp \
	Grabber.cpp \
	utils.cpp utils.hpp \
	ScopedFile.cpp \
	SyncError.cpp \
	Uinput.cpp


libevdevxx_la_LIBADD = $(LIBEVDEV_LIBS)


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
