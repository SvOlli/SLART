#!/bin/sh

# Debian based systems
if [ -x /usr/bin/apt-get ]; then
  if [ `/usr/bin/id -u` -ne 0 ]; then
    echo "this script needs to be run as root"
    exit 1
  fi

  /usr/bin/apt-get install \
    g++ \
    libqt4-dev \
    libqt4-sql-sqlite \
    libasound2-dev \
    libvorbis-dev libsndfile1-dev \
    libcdio-paranoia-dev \
    liblircclient-dev \
    dpkg-buildpackage \
    build-essentials \
    fakeroot \
    debhelper \

  exit 0
fi

# Tiny Core Linux
if [ -x /usr/bin/tce-wget ]; then
  tce=tce
  /usr/bin/tce-wget alsa.${tce}l
  /usr/bin/tce-wget alsa-devs.${tce}
  /usr/bin/tce-wget compiletc.${tce}
  /usr/bin/tce-wget fontconfig.${tce}l
  /usr/bin/tce-wget fontconfig-devs.${tce}
  /usr/bin/tce-wget libcdio.${tce}l
  /usr/bin/tce-wget libcdio-devs.${tce}
  /usr/bin/tce-wget libsndfile.${tce}l
  /usr/bin/tce-wget libsndfile-devs.${tce}
  /usr/bin/tce-wget lirc.${tce}ml
  /usr/bin/tce-wget ogg-vorbis.${tce}l
  /usr/bin/tce-wget ogg-vorbis-devs.${tce}
  /usr/bin/tce-wget qt-4.5-sql.${tce}l
  /usr/bin/tce-wget qt-4.5-devs.${tce}l
  
  exit 0
fi

echo "Sorry, you're using a distribution that's unknown to this script."
echo "Feel free to add the necessary code and submit it to me."
echo "Thanks, SvOlli"
