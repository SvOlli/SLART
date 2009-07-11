#!/bin/sh

program=slart
version=$($(dirname $0)/../../configure --version)
major=$(echo $version|cut -f1 -d-)
revision=$(echo $version|cut -f2 -d-)

set -e
#sanity checks
if [ ${revision} -eq 0 ]; then
  echo "building a debian package is only possible when using the git version of"
  echo "$program instead of a tarball."
  exit 1;
fi

if [ ! -d extra/debian ]; then
  echo "used only from Makefile, no need to call it from elsewhere."
  exit 1
fi

#preparing environment
if [ ! -e ../slart-${version} ]; then
  ln -sv SLART ../slart-${version}
fi
#pushd ../slart-${version}
rm -rf debian
mkdir debian

#preparing debian directory
if dpkg -p libqt4-sql-sqlite 2>/dev/null >/dev/null ; then
  QT4SQL="libqt4-gui (>= 4.3), libqt4-sql-sqlite (>= 4.3)"
else
  QT4SQL="libqt4-gui (>= 4.3)"
fi

echo >&2 "populating debian directory:"
for i in $(cd extra/debian;ls *|grep -v '\.sh$');do
  echo >&2 -n "$i... "
  sed \
    -e "s@_QT4SQL_@${QT4SQL}@g" \
    <extra/debian/$i >debian/$i
done

chmod a+x debian/rules

#generate changelog
mode=0
header=
revision="`expr ${revision} + 1`"

echo >&2 "changelog..."

(git-log --date=rfc 2>/dev/null;echo) | (while read line; do
case "${line}" in
commit\ *)
  header=
  revision="`expr ${revision} - 1`"
;;
Author:*)
  author="${line#*:}"
;;
Date:*)
  date="${line#*:  }"
;;
"")
  if [ ${mode} -eq 0 ]; then
    mode=1
  else
    mode=0
    echo
    echo " --${author} ${date}"
    echo
  fi
;;
*)
  if [ -n "$(echo ${line}|grep 'RELEASE:')" ]; then
    version="$(echo ${line}|sed 's/.*RELEASE: //')"
    urgency=high
    stable=stable
  else
    urgency=low
    stable=unstable
  fi
  if [ -z "${header}" ]; then
    header="${program} (${major}-${revision}) ${stable}; urgency=${urgency}"
    echo "${header}"
    echo
  fi
  echo "  * ${line}" | fold -s -75 | sed 's/^[^ ]/    &/'
;;
esac
echo >&2 -n -e "\r${revision} "
#echo "[${line}]"
done) > debian/changelog
echo >&2 -e "\rdone."

exit 0

dpkg-buildpackage -b -rfakeroot

rm -f *-stamp

if [ -d debian ]; then
  rm -rf debian
fi

popd
if [ -h ../slart-${version} ]; then
  rm ../slart-${version}
fi
