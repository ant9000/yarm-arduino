#!/bin/bash -ex

VERSION=`grep version= ../platform.txt | sed 's/version=//g'`
FILENAME="yarm-arduino-${VERSION}.tar.bz2"

tar --transform "s|\.|saml|" \
  --exclude=IDE_Board_Manager \
  --exclude=.git* \
  --exclude=.idea \
  --exclude=.keep \
  --exclude=package/** \
  -cjf ${FILENAME} \
  -C .. \
  .

CHECKSUM=`sha256sum $FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $FILENAME | awk '{ print $1 }'`

cat package_yarm_index.json.template |
sed s/%%VERSION%%/${VERSION}/ |
sed s/%%FILENAME%%/${FILENAME}/ |
sed s/%%CHECKSUM%%/${CHECKSUM}/ |
sed s/%%SIZE%%/${SIZE}/ > package_yarm_index.json

if [ "$DEBUG" ]; then
  cat package_yarm_index.json |
  sed "s/https:\/\/github.com\/ant9000\/yarm-arduino\/raw\/master\/IDE_Board_Manager\//http:\/\/localhost:8000\//" > package_yarm_debug_index.json
fi

mv ${FILENAME} package_yarm_*index.json ../IDE_Board_Manager/
