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

OUTPUT_PATH="${PWD}/docs/generated/graphs"
SRC_PATH="${PWD}/src"
DOT_FONTNAME="FreeSans.ttf"
DOT_FONTSIZE="10"

starbanner()
{
  s="$(echo $*|sed 's/./*/g')"
  echo "**${s}**"
  echo "* ${*} *"
  echo "**${s}**"
}

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
   gif="${OUTPUT_PATH}/${cpp%.cpp}_fsm.gif"
   dot="${gif%.gif}.dot"
   mkdir -p "${gif%/*}"
   echo "processing ${cpp}"
   
(cat <<EOF
digraph finite_state_machine {

rankdir = LR;
#ratio = 0.5;
dpi = 60;
ratio=compress;
#size="1000,1000";
layout = dot;

start [fontname="${DOT_FONTNAME}",fontsize="${DOT_FONTSIZE}",labelfontname="${DOT_FONTNAME}",labelfontsize="${DOT_FONTSIZE}",height=0.2,width=0.4,shape=plaintext,size="20,20"];
edge [fontname="${DOT_FONTNAME}",fontsize="${DOT_FONTSIZE}",labelfontname="${DOT_FONTNAME}",labelfontsize="${DOT_FONTSIZE}",height=0.2,width=0.4];
node [fontname="${DOT_FONTNAME}",fontsize="${DOT_FONTSIZE}",height=0.2,width=0.4,shape=circle];

EOF
states "${cpp}"
initialstate "${cpp}"
transitions "${cpp}"
echo "}") | tee "${dot}" | dot -Tgif -o"${gif}"

   dot="${dot#*/generated/}"
   hpp="${cpp%.cpp}.hpp"
   [ -f "${hpp}" ] || hpp="${cpp}"
   grep -q "${dot}" "${hpp}" || echo "warning: missing reference to ${dot} in ${hpp}"
}

connects()
{
   cpp="${1}"
   gif="${OUTPUT_PATH}/${cpp%.cpp}_connect.gif"
   dot="${gif%.gif}.dot"
   mkdir -p "${gif%/*}"
   echo processing "${cpp}"

(cat <<EOF
digraph connect {
dpi = 60;
rankdir = LR;
layout = dot;
ratio = compress;
edge [fontname="${DOT_FONTNAME}",fontsize="${DOT_FONTSIZE}",labelfontname="${DOT_FONTNAME}",labelfontsize="${DOT_FONTSIZE}",height=0.2,width=0.4];
node [fontname="${DOT_FONTNAME}",fontsize="${DOT_FONTSIZE}",height=0.2,width=0.4];
EOF
   catsrc "$1" | grep ^connect |
      sed \
      -e 's@^connect@#connect@' \
      -e 's@#connect(\([^,]*\),S[A-Z]*(\([^(]*\).*,\([^,]*\),S[A-Z]*(\([^(]*\).*@"\1" -> "\3" [ label="\2:\4",fontsize="'"${DOT_FONTSIZE}"'",fontname="'"${DOT_FONTNAME}"'" ];@'
echo "}") > ${dot}
   if grep -q -- '->' ${dot}; then
      dot -Tgif -o${gif} ${dot}
   fi
   
   dot="${dot#*/generated/}"
   hpp="${cpp%.cpp}.hpp"
   [ -f "${hpp}" ] || hpp="${cpp}"
   grep -q "${dot}" "${hpp}" || echo "warning: missing reference to ${dot} in ${hpp}"
}

logfile="${OUTPUT_PATH%/graphs}/log"
echo "logging outout to ${logfile}"

mkdir -p "${OUTPUT_PATH}"
exec > "${logfile}" 2>&1

cd "${SRC_PATH}"

starbanner "generating connect graphs"
find * -name '*.cpp' -print0 | xargs -0 grep -l connect |
while read srcfile; do
   connects "${srcfile}"
done

starbanner "generating finite state machine graphs"
find * -name '*.cpp' -print0 | xargs -0 grep -l QStateMachine |
while read srcfile; do
   graph "${srcfile}"
done

starbanner "running doxygen"
(cd ../docs ; doxygen Doxyfile)
