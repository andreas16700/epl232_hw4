//
// Created by Sohaib on 21/11/2020.
//
#include "bmplib.h"
#include <math.h>
#include<string.h>

PRIVATE int getGray(byte red, byte green, byte blue);

PUBLIC void applyGrayscale(char *imageFileName) {
    byte *data = readImage(imageFileName);
    int sizeOfFile = getLongFrom4Bytes(&data[2]);
    int sizeOfHeaders = sizeOfFile - getLongFrom4Bytes(&data[34]);
    for (int i = sizeOfHeaders; i + 2 < sizeOfFile; i += 3) {
        int grayValue = getGray(data[i], data[i + 1], data[i + 2]);
        data[i] = grayValue;
        data[i + 1] = grayValue;
        data[i + 2] = grayValue;
    }

    createNewImageFile(addPrefix(imageFileName, "gray-"), data);
}

PRIVATE int getGray(byte red, byte green, byte blue) {
    return round(0.299 * red + 0.587 * green + 0.114 * blue);
}

