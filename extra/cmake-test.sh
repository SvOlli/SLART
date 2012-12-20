#!/bin/sh -e

mydir()
{
  cd "$(dirname $0)"
  if [ ! -x "$(basename $0)" ]; then
    echo >&2 "Can't find script's directory"
    exit 1
  fi
  pwd -P
}

fail()
{
   echo "failed to $@"
   exit 1;
}

testdir="$(mydir)/../cmake-test"
mkdir -p "${testdir}"
cd "${testdir}"

cpus="$(grep ^processor /proc/cpuinfo | wc -l)"
if [ "${cpus}" -gt 0 ]; then
   makeflags="-j${cpus}"
fi

for i in \
libs/CommandLine \
libs/Common \
libs/Magic/MagicEncoder \
libs/Magic/MagicEncoderFlac \
libs/Magic/MagicEncoderMp3 \
libs/Magic/MagicEncoderOgg \
libs/Magic/MagicEncoderWav \
libs/Satellite \
libs/TagLib \
tests/TestAppCodeEditor \
tests/TestAppCommandLine \
tests/TestAppDragAndDrop \
tests/TestAppIconView \
tests/TestAppSatellite \
tests/TestAppWebServer \
tests/TestAppXxtea \
tools/SettingsGenerator \
apps/Creep \
apps/Funkytown \
apps/Innuendo \
apps/Karmadrome \
apps/Notorious \
apps/Partyman \
apps/Rubberbandman \
apps/Sorcerer \
apps/Stripped \
. ; do
   rm -rf * || true
   cmake ../src -DCMAKE_BUILD_TYPE=debug || fail "cmake debug ${i}"
   make -C ${i} ${makeflags} || fail "make debug ${i}"
   make -C ${i} ${makeflags} install || fail "make debug install ${i}"
   rm -rf * || true
   cmake ../src  || fail "cmake release ${i}"
   make -C ${i} ${makeflags}  || fail "make release ${i}"
done
#todo
#libs/GlobalShortcut \
#tests/TestAppDatabaseBrowser \
#tests/TestBinary \

cd ${HOME}

rm -r "${testdir}"
