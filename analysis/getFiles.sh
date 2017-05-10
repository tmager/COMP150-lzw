#!/bin/bash

# Download test files

# War + Peace
wget -nc https://www.gutenberg.org/files/2600/2600-0.txt &

wait

# De-UTF8 files and chop them down to 256k
(cat 2600-0.txt | iconv -f utf-8 -t ascii//translit \
        | tail -c 400000 | head -c 65536 > WarAndPeace.text) &

wait
