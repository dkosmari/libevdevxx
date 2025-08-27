# tests/Makefile.am

if BUILD_EXAMPLES

AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS) \
	-I$(top_srcdir)/include


LDADD = \
	$(top_builddir)/src/libevdevxx.la \
	$(LIBEVDEV_LIBS)


noinst_PROGRAMS = circle-mouse


circle_mouse_SOURCES = circle-mouse.cpp



.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt

#all-local: company


endif BUILD_EXAMPLES
