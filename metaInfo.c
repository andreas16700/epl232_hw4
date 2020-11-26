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
// Created by Sohaib on 19/11/2020.
//
#include "bmplib.h"
#include "Shared.h"

PUBLIC void getMetaInfo(char *bmpImage) {
    byte *data = readImage(bmpImage);
    printf("BITMAP_FILE_HEADER\n"
           "==================\n");
    printf("bfType: %c%c\n", data[0], data[1]);

    printf("bfSize: %u\n", getLongFrom4Bytes(&data[2]));
    printf("bfReserved1: %u\n", getLongFrom2Bytes(&data[6]));
    printf("bfReserved2: %u\n", getLongFrom2Bytes(&data[8]));
    printf("bfOffBits: %u\n", getLongFrom4Bytes(&data[10]));
    printf("\n");
    printf("BITMAP_INFO_HEADER\n"
           "==================\n");
    printf("biSize: %u\n", getLongFrom4Bytes(&data[14]));
    printf("biWidth: %u\n", getLongFrom4Bytes(&data[18]));
    printf("biHeight: %u\n", getLongFrom4Bytes(&data[22]));
    printf("biPlanes: %u\n",getLongFrom2Bytes(&data[26]));
    printf("biBitCount: %u\n",getLongFrom2Bytes(&data[28]));
    printf("biCompression: %u\n", getLongFrom4Bytes(&data[30]));
    printf("biSizeImage: %u\n", getLongFrom4Bytes(&data[34]));
    printf("biXPelsPerMeter: %u\n", getLongFrom4Bytes(&data[38]));
    printf("biYPelsPerMeter: %u\n", getLongFrom4Bytes(&data[42]));
    printf("biClrUsed: %u\n", getLongFrom4Bytes(&data[46]));
    printf("biClrImportant: %u\n", getLongFrom4Bytes(&data[50]));
    printf("\n***************************************************************************\n");
    free(data);
}
#define DEBUG_LIST
#ifndef DEBUG_LIST
int main(int argc, char *argv[]){
    if(argc < 1)
        return 0;
    getMetaInfo(argv[1]);
    return 0;
}
#endif