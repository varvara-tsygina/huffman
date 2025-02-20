#include "huffman.h"
#include <string.h>

bool wcomp(const char* inputfilename, const char* outputfilename, NODE* root, unsigned int arr[]) {
    FILE* inFile = fopen(inputfilename, "rb");
    if (!inFile) {
        perror("Error opening input file for reading");
        return false;
    }

    FILE* outFile = fopen(outputfilename, "wb");
    if (!outFile) {
        fclose(inFile);
        perror("Error opening output file for writing");
        return false;
    }

    fseek(inFile, 0, SEEK_END);
    uint32_t originalFileSize = (uint32_t)ftell(inFile); 
    fseek(inFile, 0, SEEK_SET);

 
    HuffmanHeader header;
    header.originalFileSize = originalFileSize;
    header.paddingBits = 0; 
    memcpy(header.arr, arr, sizeof(header.arr));

    fwrite(&header, sizeof(HuffmanHeader), 1, outFile);

    unsigned char bitBuffer = 0;
    int bitCount = 0;
    int c;

    while ((c = fgetc(inFile)) != EOF) {
        unsigned char symbol = (unsigned char)c;
        NODE* node = NULL;

        NODE* stack[ALPHABET_SIZE]; 
        int stackTop = -1;

        stack[++stackTop] = root;
        while (stackTop >= 0) {
            NODE* currentNode = stack[stackTop--];

            if (currentNode->isSymb && currentNode->symb == symbol) {
                node = currentNode;
                break;
            }

            if (currentNode->right) stack[++stackTop] = currentNode->right;
            if (currentNode->left) stack[++stackTop] = currentNode->left;
        }


        if (!node) {
            fprintf(stderr, "Error: Symbol not found in Huffman tree: %c\n", symbol);
            fclose(inFile);
            fclose(outFile);
            return false;
        }
        for (int i = 0; i < node->level; ++i) {
            bitBuffer <<= 1;
            bitBuffer |= node->code[i];
            bitCount++;

            if (bitCount == 8) {
                fputc(bitBuffer, outFile);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        header.paddingBits = 8 - bitCount;
        bitBuffer <<= header.paddingBits; 
        fputc(bitBuffer, outFile);

        fseek(outFile, 0, SEEK_SET);
        fwrite(&header, sizeof(HuffmanHeader), 1, outFile);
    }
    else
    {
        fseek(outFile, 0, SEEK_SET);
        fwrite(&header, sizeof(HuffmanHeader), 1, outFile);
    }

    fclose(inFile);
    fclose(outFile);
    return true;
}