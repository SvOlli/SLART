#!/bin/bash

jobs="$(grep "processor	:" /proc/cpuinfo|wc -l)"
[ ${jobs} -gt 0] && MAKEFLAGS="-j${jobs}"

set -e

getversion()
{
  if [ -d "$1/.svn" ]; then
    (cd "$1"; echo "subversion revision $(svn info|grep '^Revision: '|cut -f2 -d:|tr -d ' ')")
  else
    echo "version ${1##*-}"
  fi
}

mpg123_dir=""
dermixd_dir=""

for arg; do
case ${arg} in
/*mpg123*)
  mpg123_dir="${arg}"
;;
*mpg123*)
  mpg123_dir="${PWD}/${arg}"
;;
/*dermixd*)
  dermixd_dir="${arg}"
;;
*dermixd*)
  dermixd_dir="${PWD}/${arg}"
;;
esac
done

echo "building from dirs:"
echo "mpg123:  ${mpg123_dir}"
echo "dermixd: ${dermixd_dir}"

if [ -z "${mpg123_dir}" -o -z "${dermixd_dir}" ]; then
  echo usage $0 mpg123-dir dermixd-dir
  exit 0
fi

target_dir="$(echo ${PWD}|sed -e 's@^\(.*/SLART[^/]*\).*@\1@' -e 's@^\(.*/slart[^/]*\).*@\1@')/build/release"
mpg123_version="$(getversion $mpg123_dir)"
dermixd_version="$(getversion $dermixd_dir)"

echo "versions:"
echo "mpg123:  ${mpg123_version}"
echo "dermixd: ${dermixd_version}"
echo "target directory: ${target_dir}"

sleep 1

mkdir -p "${target_dir}/share/doc/SLART"
cat >"${target_dir}/share/doc/SLART/README.DerMixD" <<EOF
This package contains DerMixD ${dermixd_version},
which itself contains libmpg123 ${mpg123_version} statically linked in.

They are available at http://www.mpg123.de/ and http://www.dermixd.de/.
EOF

echo
echo "=========="
echo "= mpg123 ="
echo "=========="
echo

cd "${mpg123_dir}"

make distclean 2>/dev/null >/dev/null || true

if [ -x autogen.sh -a ! -x configure ]; then
  ./autogen.sh \
    --prefix=/usr/local \
    --enable-largefile \
    --enable-static \
    --disable-shared
else
  ./configure \
    --prefix=/usr/local \
    --enable-largefile \
    --enable-static \
    --disable-shared
fi
find . -name Makefile | xargs sed -i 's/^CFLAGS = /CFLAGS = -D_FILE_OFFSET_BITS=64 /g'
make ${MAKEFLAGS}

echo
echo "==========="
echo "= DerMixD ="
echo "==========="
echo

cd "${dermixd_dir}"

rm -f dermixd*
sed -i Makefile \
    -e "s@^ADDFLAGS=.*\$@ADDFLAGS=-D_FILE_OFFSET_BITS=64 -I${mpg123_dir}/src/libmpg123@" \
    -e "s@^EXTRA_LIBS=.*\$@EXTRA_LIBS=-L${mpg123_dir}/src/libmpg123/.libs@" \
    -e 's@ \$(LDFLAGS) \(.*\)@ \1 $(LDFLAGS)@'

make clean
make ${MAKEFLAGS} SNDFILE=yes VORBISFILE=yes LIBMPG123=yes gnu-alsa
cp dermixd dermixd-alsa

rm -f param_init.o output.o
make ${MAKEFLAGS} SNDFILE=yes VORBISFILE=yes LIBMPG123=yes gnu-oss
cp dermixd dermixd-oss

strip -R .note -R .comment dermixd-alsa dermixd-oss

cp dermixd-alsa dermixd-oss "${target_dir}/bin"
ls -l "${target_dir}/bin/dermixd-alsa" "${target_dir}/bin/dermixd-oss"

