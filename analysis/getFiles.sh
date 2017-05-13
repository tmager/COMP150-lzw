#!/bin/bash

# Download test files

# War + Peace
wget -nc https://www.gutenberg.org/files/2600/2600-0.txt &

# Random data
dd if=/dev/urandom of=random.dat bs=1024 count=128

# ASCII-encoded numeric data

../bin/lzw -m vs -s numeric -o /dev/null random.dat

wait

# De-UTF8 files and chop them down to 128k
(cat 2600-0.txt | iconv -f utf-8 -t ascii//translit \
        | tail -c 400000 | head -c 131072 > WarAndPeace.text) &

(cat numeric | tail -c 400000 | head -c 131072 > numeric.dat) &

wait

# Clean up

rm numeric
