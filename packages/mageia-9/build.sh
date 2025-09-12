#!/bin/bash -x
rpmdev-spectool --directory SOURCES --get-files SPECS/libevdevxx.spec || exit 1
bm -l SPECS/libevdevxx.spec || exit 2
exit 0
