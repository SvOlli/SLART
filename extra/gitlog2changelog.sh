#!/bin/sh

mydir()
{
  cd "$(dirname $0)"
  if [ ! -x "$(basename $0)" ]; then
    echo >&2 "Can't find script's directory"
    exit 1
  fi
  pwd -P
}

cd "$(mydir)/.."


program="slart"
mode=0
header=
version="0.6"
revision=0

echo >&2 "changelog..."

(git log --reverse --date=rfc 2>/dev/null;echo) | (while read line; do
case "${line}" in
commit\ *)
  header=
  revision="$(expr ${revision} + 1)"
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
    header="${program} (${version}-${revision}) ${stable}; urgency=${urgency}"
    echo "${header}"
    echo
  fi
  echo "  * ${line}" | fold -s -75 | sed 's/^[^ ]/    &/'
;;
esac
echo >&2 -n -e "\r${revision} "
#echo "[${line}]"
done)
echo >&2 -e "\rdone."
