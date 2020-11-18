//
// Created by sohae on 18/11/2020.
//
#include "bmplib.h"
#include<string.h>

newImage encodeTextInsideAnImage(FILE *sourceImage, FILE *textToHide) {

}

char **decodeTextFromImage(FILE *imageWithHiddenText) {

}

PRIVATE int getBit(char *m, int n) {
    if (n < 0 || n > 8 * strlen(m))
        return 0;
    int byte = m[n / 8];
    int posInByte = 7 - (n % 8);
    return byte & (1 << posInByte);
}

#define DEBUG_TEXTENCODINGDECODING
#ifndef DEBUG_TEXTENCODINGDECODING
int main(int argc, char *argv[]){
    char *str = {'a','b'};
    printf("%d\n",getBit(str,0));
}
#endif