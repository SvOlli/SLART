#!/bin/sh

rawentries="COPYING Makefile Global.pri Common";

mode="$1"
if [ -z "$mode" ]; then
  echo refusing to package nothing
  exit 0
fi

case "$mode" in
SLAT|Funkytown)
rawentries="$rawentries Funkytown"
;;
esac

case "$mode" in
SLAT|Innuendo)
rawentries="$rawentries Innuendo"
;;
esac

case "$mode" in
SLAT|MySpaceHack)
rawentries="$rawentries Funkytown MySpaceHack"
;;
esac

case "$mode" in
SLAT|Partyman)
rawentries="$rawentries Partyman"
;;
esac

case "$mode" in
SLAT|Rubberbandman)
rawentries="$rawentries Rubberbandman taglib"
;;
esac

case "$mode" in
SLAT|Stripped)
rawentries="$rawentries Stripped pkgconfig.sh"
;;
esac

case "$mode" in
SLAT|TemplateApp)
rawentries="$rawentries TemplateApp"
;;
esac

cookedentries="$(echo $rawentries|tr ' ' '\n'|sort -u|sed s@^@$(basename $PWD)/@g|tr '\n' ' ')"

cd ..
set -x
tar jcvf $mode.tar.bz2 $cookedentries
