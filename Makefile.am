# ./Makefile.am


EXTRA_DIST = \
	bootstrap \
	libevdevxx.pc.in \
	libevdevxx.spec.in


SUBDIRS = \
	src \
	tests


pcfiledir = $(pkgconfigdir)
pcfile_DATA = libevdevxx.pc



.PHONY: rpm

TARBALL_NAME = $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz
RPM_TARBALL_NAME = $(HOME)/rpmbuild/SOURCES/$(TARBALL_NAME)

rpm: libevdevxx.spec dist-gzip
	[ -e $(RPM_TARBALL_NAME) ] || \
		ln -s $(PWD)/$(TARBALL_NAME) $(RPM_TARBALL_NAME)
	rpmbuild -ba $<
