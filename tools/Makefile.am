# tools/Makefile.am


AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS) \
	-I$(top_srcdir)/src


LDADD = \
	$(top_builddir)/src/libevdevxx.la \
	$(LIBEVDEV_LIBS)


bin_PROGRAMS = \
	evdevxx-query \
	evdevxx-read \
	evdevxx-remap


evdevxx_query_SOURCES = query.cpp


evdevxx_read_SOURCES = read.cpp


evdevxx_remap_SOURCES = \
	ini_file.cpp ini_file.hpp \
	remap.cpp

evdevxx_remap_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	$(CPPFLAGS) \
	$(BOOST_CPPFLAGS)

evdevxx_remap_LDFLAGS = $(BOOST_LDFLAGS)


.PHONY: company

company: compile_flags.txt

compile_flags.txt: Makefile
	echo "$(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS)" | xargs -n1 | sort -u > $(srcdir)/compile_flags.txt
