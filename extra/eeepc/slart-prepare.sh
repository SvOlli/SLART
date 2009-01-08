#!/bin/sh

debs=""

for url in \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/libc/libcddb/libcddb2_1.2.1-1_i386.deb \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/libc/libcdio/libcdio6_0.76-1_i386.deb \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/libc/libcdio/libcdio-cdda0_0.76-1_i386.deb \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/libc/libcdio/libcdio-paranoia0_0.76-1_i386.deb \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/l/lirc/liblircclient0_0.8.0-9.2_i386.deb \
http://xnv45unsup.xandros.com/4.5oc/upkg/pool/main/libs/libsndfile/libsndfile1_1.0.16-1_i386.deb
do
  pkgname=$(echo $url | sed 's@.*/\([^_]*\)_.*@\1@')
  echo "*** checking for $pkgname ***"
  if [ -z "$(dpkg --get-selections|grep "^$pkgname[ 	]*install$")" ]; then
    wget -c $url
    debs="$debs $(basename $url)"
  else
    echo "already installed"
  fi
done
if [ -n "$debs" ]; then
  echo "*** installing ***"
  sudo dpkg -i $debs
fi

