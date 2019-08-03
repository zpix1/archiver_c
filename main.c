#include <stdio.h>
#include <stdlib.h>

#include "coder.h"

int main(int argc, char** argv) {
    char* infilename = argv[2];
    char* outfilename = argv[3];

    if (argv[1][0] == 'c') {
        FILE *infile = fopen(infilename, "rb");
        FILE *outfile = fopen(outfilename, "wb");

        encode(infile, outfile);

        fclose(infile);
        fclose(outfile);
    } else if (argv[1][0] == 'd') {
        FILE *infile = fopen(infilename, "rb");
        FILE *outfile = fopen(outfilename, "wb");

        decode(infile, outfile);

        fclose(infile);
        fclose(outfile);
    } else {
        printf("c to compress, d to decompress\n");
    }
}