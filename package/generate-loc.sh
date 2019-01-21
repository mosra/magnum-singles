for i in $(ls *.h); do
    echo "**[$i]($i)** |" $(cat $i | wc -l) '|' $(echo "#include \"$i\"" | g++ -std=c++11 -P -E -x c++ - | wc -l)
done
