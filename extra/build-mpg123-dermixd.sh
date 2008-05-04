#!/bin/sh

mpg123_version=1.4.1
dermixd_version=1.6.2

mpg123=$(type -p mpg123)
dermixd=$(type -p dermixd)

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

if [ -z "$build_mpg123$build_dermixd" ]; then
  echo tools are complete, nothing needs to be build
  if [ "$1" = "force" ]; then
    build_mpg123=yes
    build_dermixd=yes
  else
    echo "if you want them to be build run '$0 force'"
    exit
  fi
fi

mkdir -p src bin
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
  cp src/mpg123 ../../bin
  cp src/mpg123 ../../bin/mpg123-$mpg123_version
  cd ..
fi

if [ -n "$build_dermixd" ]; then
  rm -rf dermixd-$dermixd_version
  tar jxf dermixd-$dermixd_version.tar.bz2
  cd dermixd-$dermixd_version
  make VORBISFILE=yes SNDFILE=yes gnu-alsa
  cp dermixd ../../bin/dermixd-alsa
  cp dermixd ../../bin/dermixd-alsa-$dermixd_version
  rm output.o
  make VORBISFILE=yes SNDFILE=yes gnu-oss
  cp dermixd ../../bin/dermixd-oss
  cp dermixd ../../bin/dermixd-oss-$dermixd_version
  cd ..
fi

cd ..
strip -R .note -R .comment bin/*
ls -l bin/*

