//
// Created by Sohaib on 18/11/2020.
//
#include "bmplib.h"
#include <string.h>
#include <stdlib.h>

#define IMAGERETURNTYPE char *

PRIVATE char *readText(FILE *textToHide);

PRIVATE int getBit(char *m, int n);

PRIVATE int *createPermutationFunction(int N, unsigned int systemkey);

newImage encodeTextInsideAnImage(FILE *sourceImage, FILE *textToHide, char *sourceImageName, int key) {
    FILE *fp = fopen("new", "wb");
    char *text = readText(textToHide);
    byte *data = readImage(sourceImage);

    //not sure if this is correct for big images
    dword sizeOfImage = data[2] - 54;

    char *permutations = createPermutationFunction(sizeOfImage, key);

    for (int i = 0; i < (i + strlen(text)) * 8; i++) {
        short bit = getBit(text, i);
        int posOfBit = permutations[i];
        data[posOfBit] & ~0x1 | permutations[i];
    }

    char *newName = strcat(strcat("new-", sourceImageName), ".bmp");
    FILE *newImage = fopen(newName, "wb");
    for (int i = 0; i < sizeOfImage; i++) {
        fputc(data[i], newImage);
    }
    return newName;
}

PRIVATE char *readText(FILE *textToHide) {
    if (textToHide == NULL) {
        printf("File of text to hide not found!\n");
        return 0;
    }
    char c;
    //creating array chars
    char *text = (char *) malloc(sizeof(char) * 10);
    int cntChar = 0;
    while ((c = fgetc(textToHide)) != EOF) {
        if (cntChar + 1 >= sizeof(text)) {
            char *temp = (char *) realloc(text, (sizeof(char) * (cntChar * 2)));
            if (temp == NULL) {
                printf("Cant allocate!\n");
                return 0;
            }
            text = temp;
        }
        text[cntChar] = c;
        cntChar++;
    }
    text[cntChar] = '\0';
    return text;
}

char **decodeTextFromImage(FILE *imageWithHiddenText, unsigned int key, int length) {
    byte *imageData = readImage(imageWithHiddenText);
    dword sizeOfImage = imageData[2] - 54;
    int *permutate = createPermutationFunction(sizeOfImage, key);
    for (int i = 0; i < (i + length) * 8; i++) {
        int posOfBbyte = permutate[i];
        data[posOfBit] & ~0x1 | permutations[i];
    }

}

int *dePermutate(int *array, int N, unsigned int systemkey) {
    srand(systemkey);
    //creates array of permutations
    int *arrayOfRand = (int *) malloc(sizeof(int) * 2 * N);
    for (int j = 0; j < 2 * N; j += 2) {
        arrayOfRand[j] = rand() % N;
        arrayOfRand[j + 1] = rand() % N;
    }
    for (int i = 2 * N - 1; i - 1 >= 0; i -= 2) {
        int temp = array[arrayOfRand[i]];
        array[arrayOfRand[i]] = array[arrayOfRand[i - 1]];
        array[arrayOfRand[i - 1]] = temp;
    }
    return array;

}

PRIVATE int getBit(char *m, int n) {
    if (n < 0 || n > 8 * strlen(m))
        return 0;
    int byte = m[n / 8];
    int posInByte = 7 - (n % 8);
    int found = byte & (1 << posInByte);
    if (found > 0)
        return 1;
    return 0;
}

PRIVATE int *createPermutationFunction(int N, unsigned int systemkey) {
    srand(systemkey);
    //creates array of permutations
    int *perm = (int *) malloc(N * sizeof(int));
    //initializes with i value
    for (int i = 0; i < N; i++) {
        perm[i] = i;
    }
    //randomly changes order
    for (int i = 0; i < N; i++) {
        int val1 = rand() % N;
        int val2 = rand() % N;
        int temp = perm[val1];
        perm[val1] = perm[val2];
        perm[val2] = temp;
    }
    return perm;
}

#define DEBUG_TEXTENCODINGDECODING
#ifndef DEBUG_TEXTENCODINGDECODING
int main(int argc, char *argv[]){
    char str = {'a', 'b', '/0'};
    printf("here %d\n", getBit(&str, 0));

}
#endif