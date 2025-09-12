# doc/Makefile.am

MD_FILES = \
	mainpage.md \
	install.md

DOC_FILES = \
	$(MD_FILES) \
	$(top_srcdir)/include/libevdevxx/AbsInfo.hpp \
	$(top_srcdir)/include/libevdevxx/basic_wrapper.hpp \
	$(top_srcdir)/include/libevdevxx/Code.hpp \
	$(top_srcdir)/include/libevdevxx/Device.hpp \
	$(top_srcdir)/include/libevdevxx/evdevxx.hpp \
	$(top_srcdir)/include/libevdevxx/Event.hpp \
	$(top_srcdir)/include/libevdevxx/Grabber.hpp \
	$(top_srcdir)/include/libevdevxx/NumberBase.hpp \
	$(top_srcdir)/include/libevdevxx/Property.hpp \
	$(top_srcdir)/include/libevdevxx/ReadFlag.hpp \
	$(top_srcdir)/include/libevdevxx/ReadStatus.hpp \
	$(top_srcdir)/include/libevdevxx/SyncError.hpp \
	$(top_srcdir)/include/libevdevxx/TypeCode.hpp \
	$(top_srcdir)/include/libevdevxx/Type.hpp \
	$(top_srcdir)/include/libevdevxx/Uinput.hpp



EXTRA_DIST = \
	COPYING.MIT \
	Doxyfile.in \
	doxygen-awesome-sidebar-only.css \
	doxygen-awesome.css \
	$(MD_FILES)


if BUILD_DOCS

BUILT_SOURCES = html


EXTRA_DIST += html


html: doxyfile.stamp


doxyfile.stamp: Doxyfile $(DOC_FILES)
	$(DOXYGEN) $<
	echo Timestamp > $@


install-data-local:
	mkdir -p "$(DESTDIR)$(htmldir)"
	cp -r html "$(DESTDIR)$(htmldir)"


uninstall-local:
	-$(RM) -r "$(DESTDIR)$(htmldir)"


CLEANFILES = doxyfile.stamp

clean-local:
	$(RM) -r html

endif BUILD_DOCS
