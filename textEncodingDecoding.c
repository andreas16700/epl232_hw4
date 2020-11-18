//
// Created by sohae on 18/11/2020.
//
#include "bmplib.h"
#include <string.h>
#include <stdlib.h>

PRIVATE char *readText(FILE *textToHide);

PRIVATE int getBit(char *m, int n);

newImage encodeTextInsideAnImage(FILE *sourceImage, FILE *textToHide);

char **decodeTextFromImage(FILE *imageWithHiddenText);

PRIVATE int *createPermutationFunction(int N, unsigned int systemkey);

newImage encodeTextInsideAnImage(FILE *sourceImage, FILE *textToHide) {
    int systemkey = 1;
    char *text = readText(textToHide);
    int paddingIndex = 0;
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


char **decodeTextFromImage(FILE *imageWithHiddenText) {

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