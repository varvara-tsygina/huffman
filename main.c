#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для получения имени файла через ввод с клавиатуры
const char* getFilenameFromInput(const char* prompt) {
    char filename[260]; // Максимальная длина имени файла
    printf("%s", prompt);
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        perror("Error reading filename");
        return NULL;
    }

    // Удаляем символ новой строки, если он есть
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }

    // Копируем имя файла в динамически выделенную память
    char* result = strdup(filename);
    if (result == NULL) {
        perror("Error allocating memory for filename");
        return NULL;
    }
    return result;
}


#ifdef _WIN32
// Заглушки для неиспользуемых функций (чтобы код компилировался)
const char* openFileDialog(const char* filter) {
    (void)filter; // Предотвращаем предупреждение о неиспользуемом параметре
    return getFilenameFromInput("Enter the input filename: ");
}

const char* saveFileDialog(const char* filter) {
    (void)filter; // Предотвращаем предупреждение о неиспользуемом параметре
    return getFilenameFromInput("Enter the output filename: ");
}
#else
// Реализация для других ОС (например, Linux), если нужно графическое окно выбора файлов
// В данном примере просто используется ввод с клавиатуры
const char* openFileDialog(const char* filter) {
    (void)filter;
    return getFilenameFromInput("Enter the input filename: ");
}

const char* saveFileDialog(const char* filter) {
    (void)filter;
    return getFilenameFromInput("Enter the output filename: ");
}
#endif

int main() {
    char action[30];
    const char* inputfilename = NULL;
    const char* outputfilename = NULL;
    FileType fileType;

    printf("Select the action (compress/decompress): ");
    scanf("%19s", action); 
    if (strcmp(action, "compress") == 0) {
        printf("Select the file for compress:\n");
        inputfilename = openFileDialog("All Files (*.*)\0*.*\0");
        if (!inputfilename) {
            fprintf(stderr, "Error: no select.\n");
            return 1;
        }

        printf("Select a location to save the compressed file:\n");
        outputfilename = saveFileDialog("All Files(*.*)\0 * .*\0");
        if (!outputfilename) {
            fprintf(stderr, "Error: no select.\n");
            free((void*)inputfilename); 
            return 1;
        }

        fileType = getfiletype(inputfilename);

        unsigned int arr[ALPHABET_SIZE];
        buildfreqtabl(inputfilename, arr);

        NODE* root = buildtree(arr);
        if (!root) {
            fprintf(stderr, "Error: Could not build Huffman tree.\n");
            free((void*)inputfilename);
            free((void*)outputfilename);
            return 1;
        }

        unsigned char code[CODE_SIZE] = { 0 };
        codegenerate(root, code, 0);

        if (!wcomp(inputfilename, outputfilename, root, arr)) {
            fprintf(stderr, "Error: Compression failed.\n");
        }
        else {
            printf("File compressed successfully.\n");
        }

        freehuffmantree(root);
        free((void*)inputfilename);
        free((void*)outputfilename);
    }
    else if (strcmp(action, "decompress") == 0) {
        printf("Select the file for decompress:\n");
        inputfilename = openFileDialog("All Files(*.*)\0 * .*\0");
        if (!inputfilename) {
            fprintf(stderr, "Error: no select.\n");
            return 1;
        }

        printf("Select a location to save the decompressed file.:\n");
        outputfilename = saveFileDialog("All Files (*.*)\0*.*\0");
        if (!outputfilename) {
            fprintf(stderr, "Error: no select.\n");
            free((void*)inputfilename);
            return 1;
        }

        fileType = getfiletype(inputfilename);

        if (!decompressFile(inputfilename, outputfilename)) {
           fprintf(stderr, "Error: Decompression failed.\n");
        }
        else {
            printf("File decompressed successfully.\n");
        }

        free((void*)inputfilename);
        free((void*)outputfilename);
    }
    else {
        fprintf(stderr, "Error: Invalid action. Use 'compress' or 'decompress'.\n");
        return 1;
    }

    return 0;
}