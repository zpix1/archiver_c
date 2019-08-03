//
// Created by zpix on 03.08.19.
//

#ifndef C_ARCHIVER_CODER_H
#define C_ARCHIVER_CODER_H

#include <stdio.h>
#include <assert.h>

#include "priority_queue.h"

#define BUFFER_SIZE 256
#define BYTE 256

#define MAGIC_BYTE 0xBA

#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)   fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

// Encode given `infile` and save the result to `outfile`
void encode(FILE* infile, FILE* outfile);

// Decode given `outfile` (encoded by encode) and save the result to `outfile`
void decode(FILE* infile, FILE* outfile);


#endif //C_ARCHIVER_CODER_H
