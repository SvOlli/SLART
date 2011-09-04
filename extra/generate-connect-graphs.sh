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

cd "$(mydir)/.."

OUTPUT_PATH="${PWD}/docs/generated-connect-graphs"
SRC_PATH="${PWD}/src"


catsrc()
{
   tr -d ' \n' < "$1" | tr ';{}' '\n'
}

connects()
{
   cpp="${1}"
   gif="${OUTPUT_PATH}/${cpp%.cpp}.gif"
   gv="${gif%.gif}.gv"
   mkdir -p "${gif%/*}"
   echo processing "${cpp}"

(cat <<EOF
digraph connect {
rankdir = LR;
layout = dot;
EOF
   catsrc "$1" | grep ^connect |
      sed \
      -e 's@^connect@#connect@' \
      -e 's@#connect(\([^,]*\),S[A-Z]*(\([^(]*\).*,\([^,]*\),S[A-Z]*(\([^(]*\).*@"\1" -> "\3" [ label = "\2:\4" ];@'
echo "}") > ${gv}
   if grep -q -- '->' ${gv}; then
      dot -Tgif -o${gif} ${gv}
   fi
}


cd "${SRC_PATH}"

find * -name '*.cpp' |
while read srcfile; do
   connects "${srcfile}"
done
