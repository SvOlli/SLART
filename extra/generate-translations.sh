#!/bin/sh -e

LANGUAGES="de"

mydir()
{
  cd "$(dirname $0)"
  if [ ! -x "$(basename $0)" ]; then
    echo >&2 "Can't find script's directory"
    exit 1
  fi
  pwd -P
}

LUPDATE="$(type -p lupdate-qt4 update|head -1)"
if [ -z "${LUPDATE}"; then
   echo "can't find lupdate"
   exit 1
fi

cd "$(mydir)/../src/resources/translations"

find ../.. -name CMakeLists.txt | xargs grep -l '_\*\.ts' |
while read cmakelists; do
   dirname="${cmakelists%/*}"
   echo "processing ${dirname}"
   tsbase="${dirname##*/}"
   for lang in ${LANGUAGES}; do
      echo "generating ${tsbase}_${lang}.ts"
      ${LUPDATE} "${dirname}" -ts "${tsbase}_${lang}.ts"
   done
   echo
done

find ../.. -name CMakeLists.txt | xargs grep -l '[^_][^\*]\.ts' |
while read cmakelists; do
   dirname="${cmakelists%/*}"
   echo "processing ${dirname}"
   tsbase="${dirname##*/}"
   echo "generating ${tsbase}.ts"
   ${LUPDATE} "${dirname}" -ts "${tsbase}.ts"
   echo
done
