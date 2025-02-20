#include "huffman.h"
#include <string.h>

NODE* addToList(NODE* head, NODE* newNode) {
    if (!head) {
        return newNode;
    }

    if (newNode->arr < head->arr) {
        newNode->next = head;
        return newNode;
    }

    NODE* current = head;
    while (current->next && newNode->arr >= current->next->arr) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Функция для создания дерева Хаффмана на основе таблицы частот
NODE* buildtree(unsigned int arr[]) {
    NODE* head = NULL;

    // Создаем узлы для всех символов с ненулевой частотой
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (arr[i] > 0) {
            NODE* newNode = (NODE*)malloc(sizeof(NODE));
            if (!newNode) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
            newNode->symb = (unsigned char)i;
            newNode->isSymb = 1;
            newNode->arr = arr[i];
            newNode->left = newNode->right = NULL;
            newNode->next = NULL;
            head = addToList(head, newNode);
        }
    }

    // Объединяем узлы до тех пор, пока не останется только корень
    while (head && head->next) {
        NODE* first = head;
        head = head->next;
        first->next = NULL;

        NODE* second = head;
        head = head->next;
        second->next = NULL;

        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if (!newNode) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        newNode->symb = 0;
        newNode->isSymb = 0;
        newNode->arr = first->arr + second->arr;
        newNode->left = first;
        newNode->right = second;
        newNode->next = NULL;

        head = addToList(head, newNode);
    }

    return head;
}

// Функция для генерации кодов Хаффмана (рекурсивная)
void codegenerate(NODE* root, unsigned char code[], int level) {
    if (!root) return;

    if (root->isSymb) {
        root->level = level;
        memcpy(root->code, code, level); // Копируем код в узел
        return;
    }

    unsigned char leftCode[CODE_SIZE];
    unsigned char rightCode[CODE_SIZE];

    memcpy(leftCode, code, level);
    memcpy(rightCode, code, level);

    leftCode[level] = 0;   // 0 для левой ветви
    rightCode[level] = 1;  // 1 для правой ветви

    codegenerate(root->left, leftCode, level + 1);
    codegenerate(root->right, rightCode, level + 1);
}

// Функция для освобождения памяти, занятой деревом Хаффмана (рекурсивная)
void freehuffmantree(NODE* root) {
    if (root) {
        freehuffmantree(root->left);
        freehuffmantree(root->right);
        free(root);
    }
}