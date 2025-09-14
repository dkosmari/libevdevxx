#!/bin/bash -x

rpmdev-spectool --directory SOURCES --get-files SPECS/libevdevxx.spec || exit 1

bm -l SPECS/libevdevxx.spec || exit 2

mkdir -p output || exit 3

mv --target-directory=output \
   SRPMS/* \
   RPMS/*/* || exit 4

exit 0
