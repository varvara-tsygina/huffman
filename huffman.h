#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> // Äëÿ strcmp, strstr
#define ALPHABET_SIZE 256
#define CODE_SIZE 256

typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned int arr;
    unsigned char code[CODE_SIZE];
    int level;
    struct node* left, * right, * next;
} NODE;

typedef struct {
    uint32_t originalFileSize;
    uint8_t paddingBits;
    unsigned int arr[ALPHABET_SIZE];
} HuffmanHeader;


typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_TXT,
    FILE_TYPE_JPG,
    FILE_TYPE_PNG,
    FILE_TYPE_BMP,
    FILE_TYPE_WAV
} FileType;

void buildfreqtabl(const char* filename, unsigned int arr[]);
NODE* buildtree(unsigned int arr[]);
void codegenerate(NODE* root, unsigned char code[], int level);
bool wcomp(const char* inputfilename, const char* outputfilename, NODE* root, unsigned int arr[]);
bool decompressFile(const char* compressedFilename, const char* decompressedFilename);
void freehuffmantree(NODE* root);
FileType getfiletype(const char* filename); 

#endif
