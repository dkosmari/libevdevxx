#!/bin/bash -x
rpmdev-spectool --directory SOURCES --get-files SPECS/libevdevxx.spec || exit 1
rpmbuild -ba SPECS/libevdevxx.spec || exit 2
exit 0
