#!/bin/sh

set -e

cd "$(dirname $0)"
if [ ! -x "$(basename $0)" ]; then
  echo "Can't find packaging directory"
  exit 1
fi

# TODO:
# - version
# - changelog
# - build DerMixD

TOPSRC="$(cd ../.. ; pwd)"
RELEASE="${TOPSRC}/build/release"
DEBIAN="${RELEASE}/DEBIAN"
EXTRA="${TOPSRC}/extra"

VERSION="$(grep SLART_VERSION ../../src/libs/Common/Version.hpp | cut -f2 -d\")"
REVISION="$(git log --pretty=oneline|wc -l)"

echo "Building SLART ${VERSION}-${REVISION}"

# create source tarball
(
  cd "${TOPSRC}"
  DIRNAME="${PWD##*/}"
  DISTDIR="slart-${VERSION}-${REVISION}"
  cd ..
  if [ -L ${DISTDIR} ]; then
    rm ${DISTDIR}
  fi
  if [ "${DIRNAME}" != "${DISTDIR}" ]; then
    ln -sf ${DIRNAME} ${DISTDIR}
  fi
  tar jcvf ${DISTDIR}.tar.bz2 $(ls -1d ${DISTDIR}/*|grep -v 'build$')
) || exit 11

# build binaries
(
  cd "${TOPSRC}"
  #make clean
  jobs=$(grep "processor	:" /proc/cpuinfo|wc -l)
  make install -j${jobs} DESTDIR=build/release/DEBIAN/root PREFIX=/usr
) || exit 12

# build dermixd
(
  dermixd="$(ls -dt ${TOPSRC}/../dermixd*|grep -v '\.tar'|head -1)"
  mpg123="$(ls -dt ${TOPSRC}/../mpg123*|grep -v '\.tar'|head -1)"
  if [ -z "${dermixd}" -o -z "${mpg123}" ]; then
    echo "can't build mpg123 or dermixd"
    exit 1
  else
    ../../extra/build-dermixd-with-libmpg123.sh ${dermixd} ${mpg123}
  fi
) || exit 13

mkdir -p "${DEBIAN}/root/usr/share/menu"
for desktop in "${EXTRA}/menu"/*.desktop; do
(
  eval $(tail -n +2 "${desktop}" | sed 's/\([^=]*\)=\(.*\)/\1="\2"/')
  echo "?package(slart):needs=\"X11\" section=\"Applications/Sound\" title=\"${Name}\" command=\"${Exec}\""
)
done > "${DEBIAN}/root/usr/share/menu/slart"

(
  cd "${DEBIAN}/root"
  find * ! -type d | xargs ls -l
  fakeroot tar zcvf ../../tmp/data.tar.gz .
) || exit 14

(
  rm -rf "${DEBIAN}/debian"
  cp -r "debian" "${DEBIAN}"
  cd "${DEBIAN}"
  dpkg-shlibdeps --warnings=7 -O "../DEBIAN/root/usr/bin"/* "../DEBIAN/root/usr/lib"/* |
    sed 's@\([^=]*\)=\(.*\)@s/${\1}/\2/@' > ../tmp/control.sed
  (cd "root" ; find * -type f | xargs md5sum) > debian/md5sums
  echo 's/\${arch}/'$(dpkg-architecture -qDEB_BUILD_ARCH)'/g' >> ../tmp/control.sed
  echo 's/\${size}/'$(du -k -s .|cut -f1)'/g' >> ../tmp/control.sed
  cd debian
  sed -f ../../tmp/control.sed -i control
  fakeroot tar zcvf ../../tmp/control.tar.gz control md5sums
) || exit 15

(
  cd "${RELEASE}/tmp"
  echo "2.0" > debian-binary
  [ -n "$(type -p advdef)" ] && advdef -z4 control.tar.gz data.tar.gz
  eval $(dpkg-architecture -s)
  ar rvc ../slart_${VERSION}-${REVISION}_${DEB_BUILD_ARCH}.deb debian-binary control.tar.gz data.tar.gz
) || exit 16

