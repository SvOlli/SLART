#!/bin/sh

program=slart
version=$(../configure --version|cut -f1-2 -d.)
revision=$(git-log 2>/dev/null | grep ^commit | wc -l)

mode=0
(git-log 2>/dev/null;echo) | while read line; do
case "${line}" in
commit\ *)
  echo "${program} (${version}-${revision}) unstable; urgency=low"
  echo
  revision=$[revision-1]
;;
Author:*)
  author="$(echo ${line}|cut -f2- -d:)"
;;
Date:*)
  date="$(echo ${line}|cut -f2- -d:)"
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
  echo "  * ${line}"
;;
esac
#echo "[${line}]"
done

