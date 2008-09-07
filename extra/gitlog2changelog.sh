#!/bin/bash

program=slart
version=$($(dirname $0)/../configure --version|cut -f1 -d-)
revision=$(git-log 2>/dev/null | grep ^commit | wc -l)

if [ ${revision} -eq 0 ]; then
  echo "This script is only useful using the git version instead of a tarball."
  exit 1;
fi

mode=0
header=
revision=$[revision+1]
(git-log 2>/dev/null;echo) | while read line; do
case "${line}" in
commit\ *)
  header=
  revision=$[revision-1]
;;
Author:*)
  author="$(echo ${line}|cut -f2- -d:)"
;;
Date:*)
  date="$(echo ${line}|cut -f2- -d:|sed 's/^\( *[A-Z][a-z][a-z]\),* \([A-Z][a-z][a-z]\) \([0-9]*\) \([0-9:]*\) \([0-9]*\) /\1, \3 \2 \5 \4 /')"
;;
"")
  mode=$[1-mode]
  if [ ${mode} -eq 0 ]; then
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
    header="${program} (${version}-${revision}) ${stable}; urgency=${urgency}"
    echo "${header}"
    echo
  fi
  echo "  * ${line}" | fold -s -75 | sed 's/^[^ ]/    &/'
;;
esac
#echo "[${line}]"
done

