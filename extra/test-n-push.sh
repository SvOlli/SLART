#!/bin/sh -e

TESTDIR="/tmp/src/SLART"

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

cd $(mydir)/..
[ -d "${TESTDIR}" ] && rm -rf "${TESTDIR}"
mkdir -p "${TESTDIR}"
git clone . "${TESTDIR}"
(cd "${TESTDIR}";make src/Makefile) || fail "prepare qmake build environment"
"${TESTDIR}/extra/cmake-test.sh"    || fail "run cmake test build"
rm -rf "${TESTDIR}"
extra/git-push.sh
