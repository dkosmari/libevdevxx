# tools/Makefile.am

if BUILD_TOOLS

AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS) \
	-I$(top_srcdir)/include


LDADD = \
	$(top_builddir)/src/libevdevxx.la \
	$(LIBEVDEV_LIBS)


bin_PROGRAMS = \
	evdevxx-query \
	evdevxx-read


evdevxx_query_SOURCES = query.cpp


evdevxx_read_SOURCES = read.cpp


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt


endif BUILD_TOOLS
