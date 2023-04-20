#!/bin/bash

FILES=$(find -name "*.[ch]")

for file in $FILES; do
    echo $file;
    clang-format $file > temp && diff $file temp && rm temp;
    if [[ $? != 0 ]]
    then
        exit 1;
    fi
done
