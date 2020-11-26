/**
    BMP Image Steganography - Hiding infomation inside BMP images, EPL232 Homework 4, UCY
    Copyright (C) 2020  Sohaib Nassar & Andreas Loizides

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
//
// Created by Sohaib on 21/11/2020.
//
#include "bmplib.h"
#include <math.h>

PRIVATE int getGray(byte red, byte green, byte blue);

PUBLIC void applyGrayscale(char *imageFileName) {
    byte *data = readImage(imageFileName);
    unsigned int sizeOfFile = getLongFrom4Bytes(&data[2]);
    unsigned int sizeOfHeaders = sizeOfFile - getLongFrom4Bytes(&data[34]);
    for (unsigned int i = sizeOfHeaders; i + 2 < sizeOfFile; i += 3) {
        int grayValue = getGray(data[i], data[i + 1], data[i + 2]);
        data[i] = grayValue;
        data[i + 1] = grayValue;
        data[i + 2] = grayValue;
    }
    char* newName = addPrefix(imageFileName, "gray-");
    createNewImageFile(newName, data);
    printf("Saved new image as \"%s\".\n",newName);
    free(data);
    free(newName);
}

PRIVATE int getGray(byte red, byte green, byte blue) {
    return (int)round(0.299 * red + 0.587 * green + 0.114 * blue);
}

