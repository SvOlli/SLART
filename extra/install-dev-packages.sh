#!/bin/sh

# Debian based systems
if [ -x /usr/bin/apt-get ]; then
  if [ `/usr/bin/id -u` -ne 0 ]; then
    echo "this script needs to be run as root"
    exit 1
  fi

  for pkg in \
    g++ \
    qmake \
    libqt4-dev \
    libqt4-sql-sqlite \
    libasound2-dev \
    libvorbis-dev libsndfile1-dev \
    libcdio-paranoia-dev \
    liblircclient-dev \
    libflac++-dev \
    libmp3lame-dev \
    libbz2-dev \
    dpkg-buildpackage \
    build-essentials \
    fakeroot \
    debhelper \

  do /usr/bin/apt-get install -y ${pkg}; done
  exit 0
fi

# Tiny Core Linux
if [ -x /usr/bin/tce-load ]; then
  /usr/bin/tce-load -w -i alsa.tcz
  /usr/bin/tce-load -w -i alsa-devs.tcz
  /usr/bin/tce-load -w -i compiletc.tcz
  /usr/bin/tce-load -w -i fontconfig.tcz
  /usr/bin/tce-load -w -i fontconfig-devs.tcz
  /usr/bin/tce-load -w -i libcdio.tcz
  /usr/bin/tce-load -w -i libcdio-devs.tcz
  /usr/bin/tce-load -w -i libsndfile.tcz
  /usr/bin/tce-load -w -i libsndfile-devs.tcz
  /usr/bin/tce-load -w -i libFLAC++.tcz
  /usr/bin/tce-load -w -i libFLAC++-devs.tcz
  /usr/bin/tce-load -w -i lame.tcz
  /usr/bin/tce-load -w -i lame-dev.tcz
  /usr/bin/tce-load -w -i lirc.tcz
  /usr/bin/tce-load -w -i ogg-vorbis.tcz
  /usr/bin/tce-load -w -i ogg-vorbis-devs.tcz
  /usr/bin/tce-load -w -i qt-4.5-sql.tcz
  /usr/bin/tce-load -w -i qt-4.5-devs.tcz
  
  exit 0
fi

cat <<EOF
Sorry, you're using a distribution that's unknown to this script.
Feel free to add the necessary code and submit it to me.

Thanks,
  SvOlli
EOF
