#!/bin/bash

MODES="fs vs"
FILES="WarAndPeace.text"
WIDTHS="12 14 16"

rm *.st

for mode in $MODES; do
    for file in $FILES; do
        for width in $WIDTHS; do
            ./lzw -c -m $mode -w $width -s $file.$mode$width.st $file &
        done
    done
done

wait
