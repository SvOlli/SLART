#!/bin/sh

echo "$(git rev-list --count HEAD).$(git show --abbrev-commit HEAD|grep '^commit'|sed -e 's/commit //')"

