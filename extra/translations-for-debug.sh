#!/bin/sh

TOPSRC="$(cd $(dirname $0)/..;pwd)"

for pro in $(grep -l TRANSLATIONS ${TOPSRC}/src/apps/*/*.pro ${TOPSRC}/src/libs/*/*.pro ${TOPSRC}/src/libs/*/*/*.pro); do
  echo -e "\nlupdate ${pro}\n"
  lupdate -no-obsolete ${pro}
done

apps="$(grep -l TRANSLATIONS ${TOPSRC}/src/apps/*/*.pro|sed -e 's@.*/@@' -e 's/\.pro$//')"
libs="$(grep -l TRANSLATIONS ${TOPSRC}/src/libs/*/*.pro ${TOPSRC}/src/libs/*/*/*.pro|sed -e 's@.*/@@' -e 's/\.pro$//')"
languages="$(grep TRANSLATIONS.*_ ${TOPSRC}/src/apps/*/*.pro ${TOPSRC}/src/libs/*/*.pro ${TOPSRC}/src/libs/*/*.pro|
             sed -e 's/[^_]*_/_/' -e 's/\.ts$//'|sort -u)"
             
for language in "" ${languages}; do
  for app in ${apps}; do
    ts="${TOPSRC}/src/translations/${app}${language}.ts"
    qm="${TOPSRC}/build/debug/bin/${app}${language}.qm"
    if [ -f "${ts}" ]; then
      lrelease -verbose "${ts}" -qm "${qm}"
    fi
  done
  allts=""
  qm="${TOPSRC}/build/debug/bin/Common${language}.qm"
  for lib in ${libs}; do
    ts="${TOPSRC}/src/translations/${lib}${language}.ts"
    if [ -f "${ts}" ]; then
      allts="${allts} ${ts}"
    fi
  done
  if [ -n "${allts}" ]; then
    lrelease -verbose ${allts} -qm "${qm}"
  fi
done
