#!/usr/bin/env bash

set -e

testFile () {
    echo "Test on $1"

    time ./c_archiver compress $1 $1.bia
    time ./c_archiver decompress $1.bia $1.uncompressed

    if cmp -s "$1" "$1.uncompressed"; then
        echo "$1 works well"
    else
        echo "$1 is not the same is $1.uncompressed"
    fi
}

testFile simple1.txt