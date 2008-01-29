#!/bin/sh

# this is a wrapper for pkg-config that's capable of one or more
# fallback solutions

# first try use pkg-config
if type pkg-config >/dev/null; then
  if pkg-config $@; then
    # Bingo! It worked, quit now
    exit 0
  fi
fi

# tried everything I could, failing now
exit 1

