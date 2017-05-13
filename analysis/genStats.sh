#!/bin/bash

EXEC="../bin/lzw"

MODES="fs vs"
FILES="WarAndPeace.text numeric.dat random.dat"
WIDTHS="12 14"

rm -f *.st

for mode in $MODES; do
    for file in $FILES; do
        for width in $WIDTHS; do
            ($EXEC -c -m $mode -w $width -s $file.$mode$width.st -o $file.$mode$width.lzw $file \
                && $EXEC -x -m $mode -w $width -o $file.$mode$width.extr $file.$mode$width.lzw) &
        done
    done
done

wait

for mode in $MODES; do
    for file in $FILES; do
        for width in $WIDTHS; do
            diff -q $file $file.$mode$width.extr
        done
    done
done
