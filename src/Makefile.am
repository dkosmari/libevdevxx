# src/Makefile.am


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(LIBEVDEV_CFLAGS)


lib_LTLIBRARIES = libevdevxx.la


libevdevxx_la_SOURCES = \
	AbsInfo.cpp \
	Code.cpp \
	Device.cpp \
	error.cpp error.hpp \
	Event.cpp \
	Grabber.cpp \
	Property.cpp \
	ReadFlag.cpp \
	ReadStatus.cpp \
	ScopedFile.cpp \
	SyncError.cpp \
	Type.cpp \
	TypeCode.cpp \
	Uinput.cpp \
	utils.cpp utils.hpp


libevdevxx_la_LIBADD = $(LIBEVDEV_LIBS)


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
