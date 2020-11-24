//
// Created by Sohaib on 18/11/2020.
//
#include "bmplib.h"
#include <string.h>
#include <stdlib.h>
#include "Shared.h"

//reads the text file from the given file name and returns a string
PRIVATE char *readText(char *textToHide);

//gets the bit at a position in a string
PRIVATE int getBit(char *m, int n);

//creates a permutation array with N size and srand key key
PRIVATE int *createPermutationFunction(int N, int key);

//modifies the bit at pos p to b in given byte
PRIVATE byte modifyBit(byte n, int p, int b);


PUBLIC void encodeTextInsideAnImage(char *sourceImage, char *textToHide, int key) {
    char *text = readText(textToHide);
    byte *data = readImage(sourceImage);
    int textLength = (int)strlen(text);
    printf("Length: %d\n",textLength);
    int sizeOfImage = (signed)getLongFrom4Bytes(&data[34]);

    int *permutations = createPermutationFunction(sizeOfImage, key);
    for (int i = 0; i < textLength * 8; i++) {
        int bit = getBit(text, i);
        //gets the position of the byte that the bit is going to be encoded in
        int posOfByte = permutations[i];
        //encodes bit
        data[54 + posOfByte] = modifyBit(data[54 + posOfByte], 0, bit);
    }
    free(permutations);
    createNewImageFile(addPrefix(sourceImage, "new-"), data);
    free(text);
    free(data);
}

PRIVATE byte modifyBit(byte n, int p, int b) {
    byte mask = (unsigned)1 << (unsigned)p;
    return (n & (unsigned)~mask) | (((unsigned)b << (unsigned)p) & mask);
}

PRIVATE char *readText(char *textToHide) {
    FILE *fp = fopen(textToHide, "r");
    ensureFileExists(fp, textToHide);
    char c;
    //creating array chars
    char *text = (char *) malloc(sizeof(char) * 10);
    ensureNotNull(text);
    int cntChar = 0;
    int signedByte;
    while ((signedByte = fgetc(fp)) != EOF) {
        c=(char)signedByte;
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

    int sizeOfImage = (signed) getLongFrom4Bytes(&imageData[34]);

    int *permutations = createPermutationFunction(sizeOfImage, key);

    char *decodedText = (char *) malloc(sizeof(char) * (length + 1));
    ensureNotNull(decodedText);
    unsigned int bitCnt = 7;
    int charCounter = 0;
    byte charTemp = 0;

    for (int i = 0; i < length * 8; i++) {
        //gets position of specific byte in which i bit was saved in
        int posOfByte = permutations[i];
        //gets the bit from the byte
        byte bit = imageData[posOfByte + 54] & (unsigned) 0x1;
        //bit shifted to position
        bit = bit << bitCnt;
        charTemp = charTemp | bit;

        //byte is fully read
        //moving to next byte
        if (bitCnt == 0) {
            bitCnt = 7;
            //saving byte in array
            decodedText[charCounter] = charTemp;
            charCounter++;
            charTemp = 0;
        }else{
            bitCnt--;
        }
    }
    free(permutations);
    decodedText[length] = '\0';
    //creates text file from decoded string
    createNewTextFile(newFileName, decodedText, length);
    free(decodedText);
}

PRIVATE int getBit(char *m, int n) {
    if (n < 0 || n > 8 * strlen(m))
        return 0;
    byte byt = m[n / 8];
    byte posInByte = 7 - (n % 8);
    byte found = byt & ((unsigned) 1 << posInByte);
    if (found > 0)
        return 1;
    return 0;
}

PRIVATE int *createPermutationFunction(int N, int key) {
    srand48(key);
    //creates array of permutations
    int *perm = (int *) malloc(N * sizeof(int));
    ensureNotNull(perm);
    //initializes with i value
    for (int i = 0; i < N; i++) {
        perm[i] = i;
    }
    //randomly changes order
    for (int i = 0; i < N; i++) {
        int val1 = (int)(lrand48() % N);
        int val2 = (int)(lrand48() % N);
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