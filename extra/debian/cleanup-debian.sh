#!/bin/bash

program=slart
version=$($(dirname $0)/../../configure --version)
major=$(echo $version|cut -f1 -d-)
revision=$(echo $version|cut -f2 -d-)

set -e
#sanity check
if [ ! -d extra/debian ]; then
  echo "used only from Makefile, no need to call it from elsewhere."
  exit 1
fi

rm -f *-stamp

if [ -d debian ]; then
  rm -rf debian
fi

if [ -h ../slart-${version} ]; then
  rm ../slart-${version}
fi
