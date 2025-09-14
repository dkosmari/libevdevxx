#!/bin/bash -x

NAME=libevdevxx
VERSION=0.5.2
TARNAME=${NAME}-${VERSION}
TARBALL=${TARNAME}.tar.gz

URL="https://github.com/dkosmari/libevdevxx/releases/download/v${VERSION}/${TARBALL}"

wget -q "${URL}" || exit 1

ln -s ${TARBALL} ${NAME}_${VERSION}.orig.tar.gz || exit 2

tar xf ${TARBALL} || exit 3

(cd ${TARNAME} && mv ../debian . && debuild) || exit 4

mkdir -p output || exit 5

mv --target-directory=output \
   *.deb \
   *.diff.gz \
   *.dsc \
   *.build \
   *.buildinfo \
   *.changes || exit 6

exit 0
