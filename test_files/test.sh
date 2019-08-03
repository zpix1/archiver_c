#!/usr/bin/env bash

set -e

testFile () {
    COMPRESS_TIME=`/bin/time -f "%E" ./$1 compress $2 $2.bia 2>&1`
    EXTRACT_TIME=`/bin/time -f "%E" ./$1 decompress $2.bia $2.extracted 2>&1`

    ORIGINAL_FS=`stat --printf='%s' $2`
    COMPRESSED_FS=`stat --printf='%s' $2.bia`

    printf "Testing $2; C=$COMPRESS_TIME; E=$EXTRACT_TIME; `awk "BEGIN { print \"$COMPRESSED_FS/$ORIGINAL_FS\" }"`; ($ORIGINAL_FS->$COMPRESSED_FS) "

#    if

    if cmp -s "$2" "$2.extracted"; then
        echo -e "\033[32m$2 match\e[0m"
    else
        echo -e "\033[34m$2 difference \e[0m"
    fi


}

testFile archiver_c simple1.txt
testFile archiver_c simple2.txt

# To use these tests download a big file, an image and a very big image to test it
# Because these files are not included into git
#testFile archiver_c 100MB.bin
#testFile archiver_c image.png
#testFile archiver_c bigimage.jpg