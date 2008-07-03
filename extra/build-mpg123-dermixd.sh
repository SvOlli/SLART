#!/bin/bash

mpg123_version=1.4.3
dermixd_version=1.6.2
dermixd_options="VORBISFILE=yes SNDFILE=yes"

mpg123=$(type -p mpg123 2>/dev/null)
dermixd=$(type -p dermixd 2>/dev/null)

if [ "$(dirname $PWD|sed 's@^.*/@@g')" = "SLART" ]; then
bindir=../$(uname -s -m|tr ' ' _)/bin
else
bindir=bin
fi

if [ -z "$mpg123" ]; then
  echo "no mpg123 found, need to build one"
  build_mpg123=yes
else
  echo "mpg123 is $mpg123"
  mpg123_name=$($mpg123 --version|head -1|cut -f1 -d' ')
  if [ "$mpg123_name" = "mpg321" ]; then
    echo "only mpg321 found, which is NOT compatible, need to build mpg123"
    build_mpg123=yes
  fi
fi

if [ -z "$dermixd" ]; then
  echo "no dermixd found, need to build one"
  build_dermixd=yes
else
  echo "dermixd is $dermixd"
fi

if [ "$1" = "force" ]; then
  build_mpg123=yes
  build_dermixd=yes
fi

if [ -z "$build_mpg123$build_dermixd" ]; then
  echo tools are complete, nothing needs to be build
  echo "if you want them to be build run '$0 force'"
  exit
fi

echo "will copy compiled mpg123 $mpg123_version and dermixd $dermixd_version to $bindir"
mkdir -p src $bindir
cd src

if [ -n "$build_mpg123" ]; then
  if [ ! -f mpg123-$mpg123_version.tar.bz2 ]; then
    wget http://mpg123.de/download/mpg123-$mpg123_version.tar.bz2
  fi
fi

if [ -n "$build_dermixd" ]; then
  if [ ! -f dermixd-$dermixd_version.tar.bz2 ]; then
    wget http://thomas.orgis.org/dermixd/dermixd-$dermixd_version.tar.bz2
  fi
fi

if [ -n "$build_mpg123" ]; then
  rm -rf mpg123-$mpg123_version
  tar jxf mpg123-$mpg123_version.tar.bz2
  cd mpg123-$mpg123_version
  ./configure --prefix=/usr/local --disable-shared --enable-static && make
  cp src/mpg123 ../../$bindir/mpg123-$mpg123_version
  ln -sf mpg123-$mpg123_version ../../$bindir/mpg123
  cd ..
fi

if [ -n "$build_dermixd" ]; then
  rm -rf dermixd-$dermixd_version
  tar jxf dermixd-$dermixd_version.tar.bz2
  cd dermixd-$dermixd_version
  make $dermixd_options gnu-oss
  cp dermixd ../../$bindir/dermixd-oss-$dermixd_version
  ln -sf dermixd-oss-$dermixd_version ../../$bindir/dermixd-oss
  rm output.o param_init.o
  make $dermixd_options gnu-alsa
  cp dermixd ../../$bindir/dermixd-alsa-$dermixd_version
  ln -sf dermixd-alsa-$dermixd_version ../../$bindir/dermixd-alsa
  cd ..
fi

cd ..
strip -R .note -R .comment $bindir/{mpg123,dermixd}*
(cd $bindir;ls -l {mpg123,dermixd}*)

