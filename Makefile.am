# ./Makefile.am

ACLOCAL_AMFLAGS = -I m4


EXTRA_DIST = \
	bootstrap \
	libevdevxx.pc.in \
	libevdevxx.spec.in \
	README.md


SUBDIRS = \
	include/libevdevxx \
	src \
	examples \
	tools \
	doc


pcfiledir = $(pkgconfigdir)
pcfile_DATA = libevdevxx.pc



.PHONY: rpm

RPM_TARBALL_NAME = $(HOME)/rpmbuild/SOURCES/$(TARBALL_NAME)

rpm: libevdevxx.spec dist-gzip
	[ -e $(RPM_TARBALL_NAME) ] || \
		$(LN_S) $(PWD)/$(TARBALL_NAME) $(RPM_TARBALL_NAME)
	rpmbuild -ba $<
