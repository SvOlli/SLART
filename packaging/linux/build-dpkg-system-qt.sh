#!/bin/sh

# TODO:
# - version
# - changelog
# - build DerMixD

TOPSRC="$(cd ../.. ; pwd)"
RELEASE="${TOPSRC}/build/release"
DEBIAN="${RELEASE}/DEBIAN"
EXTRA="${TOPSRC}/extra"

(cd "${TOPSRC}"
 #make clean
 jobs=$(grep "processor	:" /proc/cpuinfo|wc -l)
 make install -j${jobs} DESTDIR=build/release/DEBIAN/root PREFIX=/usr)

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
)

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
)

(
  cd "${RELEASE}/tmp"
  echo "2.0" > debian-binary
  [ -n "$(type -p advdef)" ] && advdef -z4 control.tar.gz data.tar.gz
  ar rvc ../slart.deb debian-binary control.tar.gz data.tar.gz
)
