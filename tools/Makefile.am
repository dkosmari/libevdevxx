# tools/Makefile.am

if BUILD_TOOLS

AM_CXXFLAGS = -Wall -Wextra


AM_CPPFLAGS = \
	$(LIBEVDEV_CFLAGS) \
	-I$(top_srcdir)/include


LDADD = ../libevdevxx.la


bin_PROGRAMS = \
	evdevxx-query \
	evdevxx-read


evdevxx_query_SOURCES = query.cpp


evdevxx_read_SOURCES = read.cpp


# Don't leave empty directories behind during uninstall
uninstall-hook:
	-rmdir --ignore-fail-on-non-empty --parent $(DESTDIR)$(bindir)


.PHONY: company
company: compile_flags.txt

compile_flags.txt: Makefile
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt


endif BUILD_TOOLS
