//
// Created by Sohaib on 18/11/2020.
//
#include "bmplib.h"
#include <string.h>
#include <stdlib.h>
#include "Shared.h"

PRIVATE char *readText(char *textToHide);

PRIVATE int getBit(char *m, int n);

PRIVATE int *createPermutationFunction(int N, int key);

PRIVATE byte modifyBit(byte n, int p, int b);

PUBLIC void encodeTextInsideAnImage(char *sourceImage, char *textToHide, int key) {
    char *text = readText(textToHide);
    byte *data = readImage(sourceImage);

    unsigned long sizeOfImage = getLongFrom4Bytes(&data[34]);

    int *permutations = createPermutationFunction(sizeOfImage, key);
    for (int i = 0; i < strlen(text) * 8; i++) {
        int bit = getBit(text, i);
        int posOfByte = permutations[i];
        data[54 + posOfByte] = modifyBit(data[54 + posOfByte], 0, bit);
    }
    createNewImageFile(addPrefix(sourceImage, "new-"), data);
}

PRIVATE byte modifyBit(byte n, int p, int b) {
    byte mask = 1 << p;
    return (n & ~mask) | ((b << p) & mask);
}

PRIVATE char *readText(char *textToHide) {
    FILE *fp = fopen(textToHide, "r");
    ensureNotNull(fp);
    char c;
    //creating array chars
    char *text = (char *) malloc(sizeof(char) * 10);
    ensureNotNull(text);
    int cntChar = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (cntChar + 1 >= sizeof(text)) {
            char *temp = (char *) realloc(text, (sizeof(char) * (cntChar * 2)));
            ensureNotNull(temp);
            text = temp;
        }
        text[cntChar] = c;
        cntChar++;
    }
    text[cntChar] = '\0';
    fclose(fp);
    return text;
}

PUBLIC void decodeTextFromImage(char *imageWithHiddenText, char *newFileName, int key, int length) {
    byte *imageData = readImage(imageWithHiddenText);

    unsigned long sizeOfImage = getLongFrom4Bytes(&imageData[34]);

    int *permutations = createPermutationFunction(sizeOfImage, key);
    char *decodedText = (char *) malloc(sizeof(char) * (length + 1));
    ensureNotNull(decodedText);
    int bitCnt = 7;
    int charCounter = 0;
    byte charTemp = 0;
    for (int i = 0; i < length * 8; i++) {
        int posOfByte = permutations[i];
        int bit = imageData[posOfByte + 54] & 0x1;
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
    createNewTextFile(newFileName, decodedText, length);
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

PRIVATE int *createPermutationFunction(int N, int key) {
    srand(key);
    //creates array of permutations
    int *perm = (int *) malloc(N * sizeof(int));
    ensureNotNull(perm);
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