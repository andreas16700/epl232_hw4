//
// Created by Andreas Loizides on 19/11/2020.
//

#include <stdlib.h>
#include "Shared.h"
void ensureNotNull(void* pntr){
    if (pntr==NULL) {
        printf("Null pointer!\n");
        exit(-1);
    }
}
int isValidBMP(FILE* file){
    //TODO other checks here
    return 1;
}
void ensureIsValidBMP(FILE* image){
    ensureNotNull(image);
    if (!isValidBMP(image)){
        printf("File is not a BMP image.\n");
        exit(-1);
    }

}
char* addPrefix(const char* current, const char* prefix){
    int newSize = (int) strlen(current) + (int) strlen(prefix);
    char* newName = malloc(newSize+1);
    ensureNotNull(newName);
    strcat(newName,prefix);
    strcat(newName,current);
    //just to be sure...
    if (newName[newSize]!='\0')
        newName[newSize]='\0';
    return newName;
}
unsigned long getLongFrom4Bytes(byte *b) {
    unsigned long retval = (unsigned long) b[3] << 24 | (unsigned long) b[2] << 16;
    retval |= (unsigned long) b[1] << 8 | b[0];
    return retval;
}

unsigned long getLongFrom2Bytes(byte *b) {
    unsigned long retval = (unsigned long) b[2] << 8;
    retval |= b[0];
    return retval;
}
int intFromNBytes(const byte* bytes, int n){
    unsigned int num=0;
    for (int i = n-1; i >= 0 ; --i) {
        num<<=(unsigned)8;
        num|=bytes[i];
    }
    return (signed)num;
}
void copyHeader(FILE* from, FILE* to){
    rewind(from);
    rewind(to);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i) {
        byte headerByte = fgetc(from);
        fputc(headerByte,to);
    }
}
Dimensions readDimensionsOfImage(FILE* image){
    rewind(image);
    //skip file header
    int fileHeaderLength=14, infoHeaderBytesToSkip=4;
    int bytesToSkip = fileHeaderLength+infoHeaderBytesToSkip;
    int dummy;
    for (int i = 0; i < bytesToSkip; ++i)
        dummy=fgetc(image);
    byte* biWidth = malloc(4*sizeof(byte));
    byte* biHeight = malloc(4*sizeof(byte));
    for (int i = 0; i <4; ++i)
        biWidth[i]=fgetc(image);
    for (int i = 0; i <4; ++i)
        biHeight[i]=fgetc(image);
    Dimensions dimensions={.biHeight=intFromNBytes(biHeight,4), .biWidth=intFromNBytes(biWidth,4)};
    return dimensions;
}