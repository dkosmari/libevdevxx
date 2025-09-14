#!/bin/bash -x

NAME=libevdevxx
VERSION=0.5.2
TARNAME=${NAME}-${VERSION}
TARBALL=${TARNAME}.tar.gz

URL="https://github.com/dkosmari/libevdevxx/releases/download/v${VERSION}/${TARBALL}"

wget -q "${URL}" || exit 1

ln -s ${TARBALL} ${NAME}_${VERSION}.orig.tar.gz

tar xf ${TARBALL} || exit 2

(cd ${TARNAME} && mv ../debian . && debuild) || exit 3

mkdir -p output || exit 4

mv --target-directory=output \
   *.deb \
   *.diff.gz \
   *.dsc \
   *.build \
   *.buildinfo \
   *.changes || exit 5

exit 0
