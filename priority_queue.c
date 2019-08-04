//
// Created by zpix on 03.08.19.
//

#include "priority_queue.h"

struct freq_byte* new_freq_byte(int freq, unsigned char value) {
    struct freq_byte* t = (struct freq_byte*)malloc(sizeof(struct freq_byte));
    t->freq = freq;
    t->value = value;
    t->next = NULL;
    return t;
}

struct freq_byte* pop(struct freq_byte** head) {
    struct freq_byte* t = *head;
    (*head) = (*head)->next;
    return t;
}

void push(struct freq_byte** head, struct freq_byte* element) {
    struct freq_byte* start = (*head);
    if ((*head)->freq > element->freq) {
        element->next = *head;
        (*head) = element;
    }
    else {
        while (start->next != NULL && start->next->freq < element->freq) {
            start = start->next;
        }
        element->next = start->next;
        start->next = element;
    }
}