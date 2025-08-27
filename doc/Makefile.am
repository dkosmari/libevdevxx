# doc/Makefile.am

MD_FILES = \
	mainpage.md \
	install.md


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

doxyfile.stamp: Doxyfile $(MD_FILES)
	$(DOXYGEN) $<
	echo Timestamp > $@

install-data-local:
	mkdir -p "$(DESTDIR)$(htmldir)"
	cp -r -t "$(DESTDIR)$(htmldir)" html

uninstall-local:
	$(RM) -r "$(DESTDIR)$(htmldir)"

CLEANFILES = doxyfile.stamp

clean-local:
	$(RM) -rf html


endif BUILD_DOCS
