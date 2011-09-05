#!/bin/sh

# will break more than it fixes, here only for documentation purposes
exit 0

git reset --hard

for srcdir in src/apps/* src/libs/C* src/libs/Magic/* src/libs/S*; do
echo ${srcdir}

find ${srcdir} -name \*.hpp | xargs grep -l '\\addtogroup' |
while read file; do
   mv -v ${file} ${file}.done
done

find ${srcdir} -name \*.hpp | xargs grep -l '/\* .* \*/' | xargs sed -i \
   -e 's@/\* \(.*headers\) \*/@// \1@' \
   -e 's@/\* \(.*classes\) \*/@// \1@' \
   -e 's@/\* \(.*class\) \*/@// \1@' \
   -e 's@/\* \(.*defaults\) \*/@// \1@' \
   2>/dev/null
   
find ${srcdir} -name \*.hpp | xargs grep -l '/\* .* \*/' | xargs sed -i \
   -e 's@/\* \(.*\) \*/@/*!\n    \\brief \1\n\n   */@' \
   2>/dev/null

find ${srcdir} -name \*.hpp | xargs grep -l '//' | xargs sed -i \
   -e 's@// \(.*headers\)@/* \1 */@' \
   -e 's@// \(.*classes\)@/* \1 */@' \
   -e 's@// \(.*class\)@/* \1 */@' \
   -e 's@// \(.*defaults\)@/* \1 */@' \
   2>/dev/null

find ${srcdir} -name \*.hpp | xargs grep -l -v '\addtogroup' | xargs sed -i \
   -e 's@^class \([^;]*\)$@/*!\n  \\addtogroup '${srcdir##*/}'\n\n  \@{\n*/\n\n/*!\n \\brief TODO\n\n*/\nclass \1@' \
   -e 's@^};$@};\n\n/*! \@} */@' \
   2>/dev/null

find ${srcdir} -name \*.hpp.done | 
while read file; do
   mv -v ${file} ${file%.done}
done

done

sed 's@addtogroup MagicEncoderInterface.hpp@addtogroup MagicEncoder@' -i \
   src/libs/Magic/MagicEncoderInterface.hpp
cp ~/src/SLART/docs/Doxyfile docs
cd docs
doxygen Doxyfile
