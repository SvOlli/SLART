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

OUTPUT_PATH="${PWD}/docs/generated/fsm-graphs"
SRC_PATH="${PWD}/src"

catsrc()
{
   tr -d ' \n' < "$1" | tr ';{}' '\n'
}

states()
{
   echo "#states"
   catsrc "$1" |
      tr ',' '\n' | 
      grep 'newQState(' | 
      cut -f1 -d'(' |
      sed -e 's@^mpState@@' -e 's@$@;@'
}

initialstate()
{
   echo "#initialstate"
   catsrc "$1" | 
      grep setInitialState |
      sed 's/.*mpState\([^ )]*\).*/start -> \1;/'
}

transitions()
{
   echo "#transitions"
   catsrc "$1" |
      grep addTransition |
      sed \
      -e 's@.*mpState\([^-]*\)->addTransition( *\([^,]*\),.*SIGNAL.\([^(]*\).*mpState\([^ )]*\).*$@\1 -> \4 [ label = "\2:\3" ];@g' \
      -e 's@this:@@g'
}

graph()
{
   cpp="${1}"
   gif="${OUTPUT_PATH}/${cpp%.cpp}.gif"
   gv="${gif%.gif}.gv"
   mkdir -p "${gif%/*}"
   echo processing "${cpp}"
   
(cat <<EOF
digraph finite_state_machine {

rankdir = LR;
#ratio = 0.5;
dpi = 60;
ratio=compress;
size="1000,1000";
layout = dot;

start [ shape = plaintext, size="20,20" ];
node [shape = circle];

EOF
states "${cpp}"
initialstate "${cpp}"
transitions "${cpp}"
echo "}") | tee "${gv}" | dot -Tgif -o"${gif}"
}

cd "${SRC_PATH}"

find * -name '*.cpp' -print0 | xargs -0 grep -l QStateMachine |
while read srcfile; do
   graph "${srcfile}"
done
