#include <stdio.h>
#include <stdlib.h>

#include "coder.h"

int is_file_empty(FILE* fp) {
    if (fp != NULL) {
        fseek (fp, 0, SEEK_END);
        int size = ftell(fp);
        if (size == 0) {
            return 1;
        }
    } else {
        return 1;
    }
    return 0;
}

void arguments_error() {
    printf("Usage:\nto compress: ./archiver_c compress <inputfile> <outputfile>\n");
    printf("to extract: ./archiver_c decompress <inputfile> <outputfile>\n");
    DEBUG_PRINT("Wrong argument passed\n");
    exit(0);
}

int main(int argc, char** argv) {
    char* infilename = argv[2];
    char* outfilename = argv[3];

    if (argc < 4) {
        arguments_error();
    }

    if (argv[1][0] == 'c') {
        DEBUG_PRINT("Compressing %s to %s\n", infilename, outfilename);
        FILE *infile = fopen(infilename, "rb");

        if (is_file_empty(infile)) {
            printf("%s is empty or does not exist, provide existing non-empty file to continue.\n", infilename);
            return 1;
        }

        FILE *outfile = fopen(outfilename, "wb");

        encode(infile, outfile);

        fclose(infile);
        fclose(outfile);
        DEBUG_PRINT("Done\n");
    } else if (argv[1][0] == 'd') {
        DEBUG_PRINT("Extracting from %s to %s\n", infilename, outfilename);
        FILE *infile = fopen(infilename, "rb");

        if (is_file_empty(infile)) {
            printf("%s is empty or does not exist, provide existing non-empty file to continue.\n", infilename);
            return 1;
        }

        FILE *outfile = fopen(outfilename, "wb");

        decode(infile, outfile);

        fclose(infile);
        fclose(outfile);
        DEBUG_PRINT("Done\n");
    } else {
        arguments_error();
    }

    return 0;
}