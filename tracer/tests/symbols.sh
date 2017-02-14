#!/usr/bin/bash

find . -name "*.run*" -type f | while read fname;
do
  echo "$fname"
  nmfile=$(basename "$fname")
  nmstub=${nmfile%.*}
  nmsymb=${nmstub%.*}
  nm -o -C --line-numbers "$fname" > "${nmsymb}.nm"
done
