#!/bin/sh -x

git push --all -u github
git push --all -u origin
git push --tags -u github
git push --tags -u origin

$(dirname $(readlink -f ${0}))/../configure
