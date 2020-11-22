//
// Created by Andreas Loizides on 19/11/2020.
//

#include <stdlib.h>
#include "Shared.h"

void ensureNotNull(void *pntr) {
    if (pntr == NULL) {
        printf("Null pointer!\n");
        exit(-1);
    }
}

int isValidBMP(FILE *file) {
    //TODO other checks here
    return 1;
}

void ensureIsValidBMP(FILE *image) {
    ensureNotNull(image);
    if (!isValidBMP(image)) {
        printf("File is not a BMP image.\n");
        exit(-1);
    }

}

char *addPrefix(const char *current, const char *prefix) {
    int newSize = (int) strlen(current) + (int) strlen(prefix);
    char *newName = malloc(newSize + 1);
    ensureNotNull(newName);
    strcat(newName, prefix);
    strcat(newName, current);
    //just to be sure...
    if (newName[newSize] != '\0')
        newName[newSize] = '\0';
    return newName;
}

int intFromNBytes(const byte *bytes, int n) {
    unsigned int num = 0;
    for (int i = n - 1; i >= 0; --i) {
        num <<= (unsigned) 8;
        num |= bytes[i];
    }
    return (signed) num;
}

unsigned int getLongFrom4Bytes(const byte *b) {
    return (unsigned) intFromNBytes(b, 4);
}

unsigned int getLongFrom2Bytes(const byte *b) {
    return (unsigned) intFromNBytes(b, 2);
}

void copyHeader(FILE *from, FILE *to) {
    rewind(from);
    rewind(to);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i) {
        byte headerByte = fgetc(from);
        fputc(headerByte, to);
    }
}

Dimensions readDimensionsOfImage(FILE *image) {
    rewind(image);
    //skip file header
    int fileHeaderLength = 14, infoHeaderBytesToSkip = 4;
    int bytesToSkip = fileHeaderLength + infoHeaderBytesToSkip;
    int dummy;
    for (int i = 0; i < bytesToSkip; ++i)
        dummy = fgetc(image);
    byte *biWidth = malloc(4 * sizeof(byte));
    byte *biHeight = malloc(4 * sizeof(byte));
    for (int i = 0; i < 4; ++i)
        biWidth[i] = fgetc(image);
    for (int i = 0; i < 4; ++i)
        biHeight[i] = fgetc(image);
    Dimensions dimensions = {.biHeight=intFromNBytes(biHeight, 4), .biWidth=intFromNBytes(biWidth, 4)};
    return dimensions;
}

byte *readOnlyImageData(FILE *image) {
    rewind(image);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i)
        fgetc(image);

    byte b;
    int readByte;
    int index = 0;
    int sizeOfArray = 54;
    byte *data = (byte *) malloc(sizeOfArray * sizeof(byte));
    readByte = fgetc(image);
    b = (byte) readByte;
    while (readByte != EOF) {
        data[index++] = b;
        if (index >= sizeOfArray) {
            sizeOfArray *= 2;
            byte *temp = (byte *) realloc(data, sizeOfArray * sizeof(byte));
            if (temp == NULL) {
                printf("Cant allocate memory!\n");
                return 0;
            }
            data = temp;
        }
        readByte = fgetc(image);
        b = (byte) readByte;
    }
    return data;
}

byte *readImage(char *imageFile) {
    FILE *fp = fopen(imageFile, "rb");
    if (fp == NULL) {
        printf("Cant read bmp file!\n");
        return 0;
    }
    byte b;
    int readByte;
    int index = 0;
    int sizeOfArray = 54;
    byte *data = (byte *) malloc(sizeOfArray * sizeof(byte));
    readByte = fgetc(fp);
    b = (byte) readByte;
    while (readByte != EOF) {

        data[index++] = b;
        if (index >= sizeOfArray) {
            sizeOfArray *= 2;
            byte *temp = (byte *) realloc(data, sizeOfArray * sizeof(byte));
            if (temp == NULL) {
                printf("Cant allocate memory!\n");
                return 0;
            }
            data = temp;
        }
        readByte = fgetc(fp);
        b = (byte) readByte;
    }
    fclose(fp);
    return data;
}

void exitWithMessage(const char *message) {
    printf("%s\n", message);
    exit(-1);
}

void createNewImageFile(char *nameOfNewImage, byte *data) {
    unsigned long sizeOfImage = getLongFrom4Bytes(data[34]);
    FILE *newImage = fopen(nameOfNewImage, "wb");
    ensureNotNull(newImage);
    for (int i = 0; i < sizeOfImage + 54; i++) {
        fputc(data[i], newImage);
    }
    fclose(newImage);
}

void createNewTextFile(char *nameOfNewTextFile, char *text, int length) {
    FILE *newText = fopen(nameOfNewTextFile, "w");
    ensureNotNull(newText);
    for (int i = 0; i < length; i++) {
        fputc(text[i], newText);
    }
    fclose(newText);
}