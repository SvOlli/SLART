#!/bin/sh

# TODO: replace with
# git describe --tags
echo "$(git rev-list HEAD|wc -l).$(git show --abbrev-commit HEAD|grep '^commit'|sed -e 's/commit //')"

