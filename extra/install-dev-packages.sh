#!/bin/sh

# Debian based systems
if [ -x "/usr/bin/apt-get" ]; then
   if [ "$(/usr/bin/id -u)" -ne 0 ]; then
      echo "this script needs to be run as root"
      exit 1
   fi

   control="$(dirname "$(readlink -f "${0}")")/../debian/control"
   pkgs="$(grep ^Build-Depends: ${control} | sed -e 's/^Build-Depends: //' -e 's/([^)]*)//g' -e 's/,//g')"

   for pkg in ${pkgs}; do
      /usr/bin/apt-get install -y "${pkg}"; done
   exit 0
fi

cat <<EOF
Sorry, you're using a distribution that's unknown to this script.
Feel free to add the necessary code and submit it to me.

Thanks,
  SvOlli
EOF
