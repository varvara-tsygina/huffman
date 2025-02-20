#include "huffman.h"
//get type
FileType getfiletype(const char* filename) {
    const char* ext = strrchr(filename, '.'); 

    if (!ext) {
        return FILE_TYPE_UNKNOWN;
    }

    if (strcmp(ext, ".txt") == 0) {
        return FILE_TYPE_TXT;
    }
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
        return FILE_TYPE_JPG;
    }
    else if (strcmp(ext, ".png") == 0) {
        return FILE_TYPE_PNG;
    }
    else if (strcmp(ext, ".bmp") == 0) {
        return FILE_TYPE_BMP;
    }
    else if (strcmp(ext, ".wav") == 0) {
        return FILE_TYPE_WAV;
    }

    return FILE_TYPE_UNKNOWN;
}

//create frequancy table

void buildfreqtabl(const char* filename, unsigned int arr[]) {
    FILE* fr = fopen(filename, "rb");
    if (!fr) {
        fprintf(stderr, "Error: Error opening file for reading\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        arr[i] = 0;
    }
    int c;
    while ((c = fgetc(fr)) != EOF) {
        arr[(unsigned char)c]++;
    }

    fclose(fr);
}