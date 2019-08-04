//
// Created by zpix on 03.08.19.
//

#ifndef C_ARCHIVER_PRIORITY_QUEUE_H
#define C_ARCHIVER_PRIORITY_QUEUE_H

#include <stdlib.h>

struct freq_byte {
    int freq;
    unsigned char value;

    int code;
    int code_length;

    struct freq_byte *left;
    struct freq_byte *right;

    struct freq_byte *next;
};

// Create a new freq_byte and allocate space for it
struct freq_byte* new_freq_byte(int freq, unsigned char value);

// Remove top element from the pq and return it
struct freq_byte* pop(struct freq_byte** head);

// Add given `element` to the pq
void push(struct freq_byte** head, struct freq_byte* element);


#endif //C_ARCHIVER_PRIORITY_QUEUE_H
