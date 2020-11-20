//
// Created by Sohaib on 18/11/2020.
//
#include "bmplib.h"
#include <string.h>
#include <stdlib.h>
//#include "metaInfo.c"

#define IMAGERETURNTYPE char *

PRIVATE char *readText(char *textToHide);

PRIVATE int getBit(char *m, int n);

PRIVATE int *createPermutationFunction(int N, unsigned int systemkey);

PRIVATE byte modifyBit(byte n, int p, int b);
//PUBLIC unsigned long getLongFrom4Bytes(byte *b);

//PUBLIC unsigned long getLongFrom2Bytes(byte *b);

void encodeTextInsideAnImage(char *sourceImage, char *textToHide, int key) {
    char *text = readText(textToHide);
    byte *data = readImage(sourceImage);

    unsigned long sizeOfImage = getLongFrom4Bytes(data[34], data[35], data[36], data[37]);

    int *permutations = createPermutationFunction(sizeOfImage, key);
    for (int i = 0; i < strlen(text) * 8; i++) {
        int bit = getBit(text, i);
        int posOfByte = permutations[i];
        data[54 + posOfByte] = modifyBit(data[54 + posOfByte], 0, bit);
    }

    char *newName = (char *) malloc(sizeof(char) * (strlen(sourceImage)) + 5);
    if (newName == NULL)
        exit(0);
    strcpy(newName, "new-");
    strcat(newName, sourceImage);
    FILE *newImage = fopen(newName, "wb");
    for (int i = 0; i < sizeOfImage + 54; i++) {
        fputc(data[i], newImage);
    }
    fclose(newImage);
}

PRIVATE byte modifyBit(byte n, int p, int b) {
    byte mask = 1 << p;
    return (n & ~mask) | ((b << p) & mask);
}

PRIVATE char *readText(char *textToHide) {
    FILE *fp = fopen(textToHide, "r");
    if (textToHide == NULL) {
        printf("File of text to hide not found!\n");
        exit(0);
    }
    char c;
    //creating array chars
    char *text = (char *) malloc(sizeof(char) * 10);
    int cntChar = 0;
    while ((c = fgetc(fp)) != EOF) {
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
    fclose(fp);
    return text;
}

char *decodeTextFromImage(char *imageWithHiddenText, unsigned int key, int length) {
    byte *imageData = readImage(imageWithHiddenText);

    unsigned long sizeOfImage = getLongFrom4Bytes(imageData[34], imageData[35], imageData[36], imageData[37]);

    int *permutations = createPermutationFunction(sizeOfImage, key);
    char *decodedText = (char *) malloc(sizeof(char) * (length + 1));
    int bitCnt = 7;
    int charCounter = 0;
    byte charTemp = 0;
    for (int i = 0; i < length * 8; i++) {
        int posOfBbyte = permutations[i];
        int bit = imageData[posOfBbyte + 54] & 0x1;
        bit = bit << bitCnt;
        charTemp = charTemp | bit;
        bitCnt--;
        if (bitCnt < 0) {
            bitCnt = 7;
            decodedText[charCounter] = charTemp;
            charCounter++;
            charTemp = 0;
        }
    }
    decodedText[length] = '\0';
    return decodedText;

}

PRIVATE int *dePermutate(int *array, int N, unsigned int systemkey) {
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
    int byt = m[n / 8];
    int posInByte = 7 - (n % 8);
    int found = byt & (1 << posInByte);
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