# tests/Makefile.am


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS) \
	-I$(top_srcdir)/src


LDADD = \
	$(top_builddir)/src/libevdevxx.la \
	$(LIBEVDEV_LIBS)


noinst_PROGRAMS = \
	circle-mouse \
	query-device \
	dump-events



circle_mouse_SOURCES = circle-mouse.cpp


query_device_SOURCES = query-device.cpp


dump_events_SOURCES = dump-events.cpp


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt

#all-local: company
