//
// Created by Sohaib on 21/11/2020.
//
#include "bmplib.h"
//#include "metaInfo.c"
#include <math.h>
#include<string.h>

int getNumberOfPaddingPixels(unsigned long biWidth);

PRIVATE void createNewImage(char *nameOfNewImage, byte *data);

int getGray(byte red, byte green, byte blue);

void applyGrayscale(char *imageFileName) {
    byte *data = readImage(imageFileName);
    //int biWidth = getLongFrom4Bytes(data[18], data[19], data[20], data[21]);
    //int biHeight = getLongFrom4Bytes(data[22], data[23], data[24], data[25]);
    //int paddingPixels = getNumberOfPaddingPixels(biWidth);
    int sizeOfFile = getLongFrom4Bytes(data[2], data[3], data[4], data[5]);
    int sizeOfHeaders = sizeOfFile - getLongFrom4Bytes(data[34], data[35], data[36], data[37]);
    for (int i = sizeOfHeaders; i + 2 < sizeOfFile; i += 3) {
        int grayValue = getGray(data[i], data[i + 1], data[i + 2]);
        data[i] = grayValue;
        data[i + 1] = grayValue;
        data[i + 2] = grayValue;
    }
    char *newName = (char *) malloc(sizeof(char) * (strlen(imageFileName)) + 5);
    if (newName == NULL)
        exit(0);
    strcpy(newName, "new-");
    strcat(newName, imageFileName);
    createNewImage(newName, data);
}

int getGray(byte red, byte green, byte blue) {
    return round(0.299 * red + 0.587 * green + 0.114 * blue);
}

int getNumberOfPaddingPixels(unsigned long biWidth) {
    if ((3 * biWidth) % 4 == 0)
        return 0;
    return 4 - ((3 * biWidth) % 4);
}

PRIVATE void createNewImage(char *nameOfNewImage, byte *data) {
    unsigned long sizeOfImage = getLongFrom4Bytes(data[34], data[35], data[36], data[37]);
    FILE *newImage = fopen(nameOfNewImage, "wb");
    for (int i = 0; i < sizeOfImage + 54; i++) {
        fputc(data[i], newImage);
    }
    fclose(newImage);
}