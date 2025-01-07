#!/bin/bash

set -e

dir=$(dirname "$0")

for i in $(ls $dir/../*.{h,hpp}); do
    echo "**[$i]($i)** |" $(cat $i | wc -l) '|' $(echo "#include \"$i\"" | g++ -std=c++11 -P -E -x c++ - | wc -l)
done
