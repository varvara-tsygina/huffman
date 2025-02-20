#include "huffman.h"

bool decompressFile(const char* compressedFilename, const char* decompressedFilename) {
    FILE* inFile = fopen(compressedFilename, "rb");
    if (!inFile) {
        perror("Error opening compressed file for reading");
        return false;
    }

    FILE* outFile = fopen(decompressedFilename, "wb");
    if (!outFile) {
        fclose(inFile);
        perror("Error opening decompressed file for writing");
        return false;
    }

    HuffmanHeader header;
    fread(&header, sizeof(HuffmanHeader), 1, inFile);

    NODE* root = buildtree(header.arr);

    if (!root) {
        fprintf(stderr, "Error: Empty Huffman tree.\n");
        fclose(inFile);
        fclose(outFile);
        return false;
    }

    NODE* currentNode = root;
    int bitCount = 0;
    unsigned char byte;
    uint32_t bytesWritten = 0;

    while (fread(&byte, 1, 1, inFile) == 1 && bytesWritten < header.originalFileSize) {
        int bitsToProcess = 8;
        if (bytesWritten + 1 >= header.originalFileSize) {
            bitsToProcess = 8 - header.paddingBits;
        }

        for (int i = 7; i >= 7 - (bitsToProcess - 1); --i) {
            int bit = (byte >> i) & 1;

            if (bit == 0) {
                currentNode = currentNode->left;
            }
            else {
                currentNode = currentNode->right;
            }

            if (currentNode->isSymb) {
                fputc(currentNode->symb, outFile);
                bytesWritten++;
                currentNode = root;
            }
        }
    }

    freehuffmantree(root);
    fclose(inFile);
    fclose(outFile);
    return true;
}