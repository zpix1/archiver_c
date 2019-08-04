//
// Created by zpix on 03.08.19.
//

#include "coder.h"

void _print_binary(int n, int length) {
    int k, c;
    for (c = length - 1; c >= 0; c--) {
        k = n >> c;
        if (k & 1) {
            DEBUG_PRINT("1");
        } else {
            DEBUG_PRINT("0");
        }
    }
    DEBUG_PRINT("\n");
}

void _print_tree(struct freq_byte *root) {
    if (root) {
        _print_tree(root->left);
        DEBUG_PRINT("%d ", root->code);
        _print_tree(root->right);
    }
}

// Generate codes for leaves
void generate_codes(struct freq_byte *top, struct freq_byte **arr) {
    if (top->left != NULL) {
        top->left->code = (top->code << 1);
        top->left->code_length = top->code_length + 1;
        generate_codes(top->left, arr);
    }
    if (top->right != NULL) {
        top->right->code = (top->code << 1) + 1;
        top->right->code_length = top->code_length + 1;
        generate_codes(top->right, arr);
    } else {
        arr[top->value] = top;
    }
}

// Free tree's memory by it's root
void free_tree(struct freq_byte *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// Save a tree structure to file by its root
void serialize(struct freq_byte *root, FILE *fp) {
    int zero = 0;
    if (root == NULL) {
        fwrite(&zero, sizeof(int), 1, fp);
        fwrite(&zero, sizeof(int), 1, fp);
        return;
    }

    fwrite(&(root->code), sizeof(int), 1, fp);
    fwrite(&(root->value), sizeof(int), 1, fp);

    serialize(root->left, fp);
    serialize(root->right, fp);
}

// Load a tree from file and return its root
struct freq_byte *deserialize(FILE *fp) {
    int code;
    int value;

    fread(&code, sizeof(int), 1, fp);
    fread(&value, sizeof(int), 1, fp);

    // Because code can't be zero at normal
    if (code == 0) {
        return NULL;
    }

    struct freq_byte *root = new_freq_byte(0, (unsigned char) value);

    root->code = code;

    root->left = deserialize(fp);
    root->right = deserialize(fp);
    return root;
}

void encode(FILE *infile, FILE *outfile) {
    fseek(infile, 0, SEEK_SET);
    fseek(outfile, 0, SEEK_SET);

    // Read file and calculate frequencies

    unsigned char buffer[BUFFER_SIZE];
    struct freq_byte *frequencies[BYTE];

    for (int i = 0; i < BYTE; i++) {
        frequencies[i] = new_freq_byte(0, (unsigned char) i);
    }

    while (1) {
        size_t amount = fread(buffer, 1, BUFFER_SIZE, infile);
        for (int i = 0; i < amount; i++) {
            frequencies[buffer[i]]->freq++;
        }
        if (amount < BUFFER_SIZE)
            break;
    }

    // Find a head for priority queue

    int head_id = -1;

    for (int i = 0; i < BYTE; i++) {
        if (frequencies[i]->freq != 0) {
            head_id = i;
            break;
        }
    }

    // Head should exists, otherwise file is empty
    assert(head_id != -1);

    DEBUG_PRINT("Pushing to pq\n");

    int counter = 0;

    for (int i = head_id + 1; i < BYTE; i++) {
        if (frequencies[i]->freq != 0) {
            counter++;
            push(&frequencies[head_id], frequencies[i]);
        }
    }

    // all chars added to pq + head
    int different_chars_count = counter + 1;

    DEBUG_PRINT("Found %d different chars\n", different_chars_count);

    DEBUG_PRINT("Popping from pq\n");

    int nodes_count = different_chars_count;
    struct freq_byte *joined;
    if (counter == 0) {
        // there is only one unique char, special case
        joined = frequencies[head_id];
    } else {
        while (1) {
            struct freq_byte *a = pop(&frequencies[head_id]);
            struct freq_byte *b = pop(&frequencies[head_id]);

            joined = new_freq_byte(a->freq + b->freq, 0);
            nodes_count++;
            joined->right = a;
            joined->left = b;

            if (counter == 1) {
                break;
            }

            push(&frequencies[head_id], joined);

            counter--;
        }
    }

    DEBUG_PRINT("Created %d nodes\n", nodes_count);
    DEBUG_PRINT("Result freq: %d \n", joined->freq);


    joined->code = 1;
    joined->code_length = different_chars_count == 0 ? 1 : 0;

    struct freq_byte *arr[BYTE];

    for (int i = 0; i < BYTE; i++) {
        arr[i] = NULL;
    }

    generate_codes(joined, arr);

#ifdef DEBUG
    DEBUG_PRINT("Codes in format <freq>(<char>/<code_length>):<code>\n");
    for (int i = 0; i < BYTE; i++) {
        if (arr[i] != NULL) {
            DEBUG_PRINT("%d(%c/%d):", arr[i]->freq, arr[i]->value, arr[i]->code_length);
            print_b(arr[i]->code, arr[i]->code_length);
        }
    }
#endif

    fseek(infile, 0, SEEK_SET);

    unsigned char current = 0;
    int bitcount = 0;

    // Write head byte (0xBA because my name is Vanya in russian)
    fputc(MAGIC_BYTE, outfile);

    // Write encoding table
    int chars_count = joined->freq;

    fwrite(&chars_count, sizeof(int), 1, outfile);
    fwrite(&nodes_count, sizeof(int), 1, outfile);

    serialize(joined, outfile);
#ifdef DEBUG
    DEBUG_PRINT("Current tree order:\n");
    print_tree(joined);
    DEBUG_PRINT("\n");
#endif

    DEBUG_PRINT("Encoding:\n");

    while (1) {
        size_t amount = fread(buffer, 1, BUFFER_SIZE, infile);
        for (int i = 0; i < amount; i++) {
            int code = arr[buffer[i]]->code;
            DEBUG_PRINT("Char:%c Code:%d\n", buffer[i], code);
            int code_length = arr[buffer[i]]->code_length;

            for (int j = 0; j < code_length; j++) {
                int bit = (code >> (code_length - j - 1)) & 1;
                current = (unsigned char) ((current << 1) | bit);
                bitcount++;
                if (bitcount == 8) {
                    fwrite(&current, 1, 1, outfile);
                    bitcount = 0;
                }
            }
        }
        if (amount < BUFFER_SIZE)
            break;
    }

    if (bitcount != 0) {
        while (bitcount != 8) {
            current <<= 1;
            bitcount++;
        }
        fwrite(&current, 1, 1, outfile);
    }

    // Free frequencies
    for (int i = 0; i < BYTE; i++) {
        if (frequencies[i] != NULL) {
            free(frequencies[i]);
        }
    }

    free_tree(joined);
}

void decode(FILE *infile, FILE *outfile) {
    fseek(infile, 0, SEEK_SET);
    fseek(outfile, 0, SEEK_SET);

    unsigned char mb = (unsigned char) fgetc(infile);
    assert(mb == MAGIC_BYTE);


    int chars_count;
    fread(&chars_count, sizeof(int), 1, infile);

    int current_chars_count = 0;
    int nodes_count;
    fread(&nodes_count, sizeof(int), 1, infile);

    DEBUG_PRINT("Found %d data sequences\n", chars_count);
    DEBUG_PRINT("Found %d nodes\n", nodes_count);

    int to_end = 0;
    struct freq_byte *top = deserialize(infile);

#ifdef DEBUG
    DEBUG_PRINT("Current tree order:\n");
    print_tree(top);
    DEBUG_PRINT("\n");
#endif

    if (nodes_count == 1) {
        for (int i = 0; i < chars_count; i++) {
            fputc(top->value, outfile);
        }
    } else {
        unsigned char buffer[BUFFER_SIZE];
        struct freq_byte *current = top;
        while (1) {
            size_t amount = fread(buffer, 1, BUFFER_SIZE, infile);
            for (int i = 0; i < amount; i++) {
                int byte = buffer[i];
                DEBUG_PRINT("Byte: %d\n", byte);
                for (int j = 0; j < 8; j++) {
                    if (current_chars_count >= chars_count) {
                        to_end = 1;
                        break;
                    }

                    int bit = (byte >> 7) & 1;
                    DEBUG_PRINT("Bit: %d\n", bit);
                    byte <<= 1;
                    DEBUG_PRINT("Current: %d\n", current);


                    if (bit == 0) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    if (current->left == NULL) {
                        current_chars_count++;
                        fputc(current->value, outfile);
                        current = top;
                    }
                }
            }
            if (amount < BUFFER_SIZE)
                break;

            if (to_end)
                break;
        }
    }

    free_tree(top);
}