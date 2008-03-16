#!/bin/sh

if [ `/usr/bin/id -u` -ne 0 ]; then
  echo "this script needs to be run as root"
  exit
fi

# Debian based systems
if [ -x /usr/bin/apt-get ]; then
  /usr/bin/apt-get install \
    libqt4-dev \
    libvorbis-dev libcdio-paranoia-dev \
    liblircclient-dev
  install=yes
fi

if [ -z "$install" ]; then
  echo "Sorry, you're using a distribution that's known to this script."
  echo "Feel free to add the necessary code and submit it to me."
  echo "Thanks, SvOlli"
fi

