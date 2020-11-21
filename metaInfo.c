//
// Created by sohae on 19/11/2020.
//
#ifndef METALINFO_
#define METALINFO_

#include "bmplib.h"

PUBLIC unsigned long getLongFrom4Bytes(byte b1, byte b2, byte b3, byte b4);

PUBLIC unsigned long getLongFrom2Bytes(byte b1, byte b2);

void getMetaInfo(char *bmpImage) {
    byte *data = readImage(bmpImage);
    printf("BITMAP_FILE_HEADER\n"
           "==================\n");
    printf("bfType: %c%c\n", data[0], data[1]);

    printf("bfSize: %lu\n", getLongFrom4Bytes(data[2], data[3], data[4], data[5]));
    printf("bfReserved1: %u\n", getLongFrom2Bytes(data[6], data[7]));
    printf("bfReserved2: %u\n", getLongFrom2Bytes(data[8], data[9]));
    printf("bfOffBits: %lu\n", getLongFrom4Bytes(data[10], data[11], data[12], data[13]));
    printf("\n");
    printf("BITMAP_INFO_HEADER\n"
           "==================\n");
    printf("biSize: %lu\n", getLongFrom4Bytes(data[14], data[15], data[16], data[17]));
    printf("biWidth: %lu\n", getLongFrom4Bytes(data[18], data[19], data[20], data[21]));
    printf("biHeight: %lu\n", getLongFrom4Bytes(data[22], data[23], data[24], data[25]));
    printf("biPlanes: %u\n", getLongFrom2Bytes(data[26], data[27]));
    printf("biBitCount: %u\n", getLongFrom2Bytes(data[28], data[29]));
    printf("biCompression: %lu\n", getLongFrom4Bytes(data[30], data[31], data[32], data[33]));
    printf("biSizeImage: %lu\n", getLongFrom4Bytes(data[34], data[35], data[36], data[37]));
    printf("biXPelsPerMeter: %lu\n", getLongFrom4Bytes(data[38], data[39], data[40], data[41]));
    printf("biYPelsPerMeter: %lu\n", getLongFrom4Bytes(data[42], data[43], data[44], data[45]));
    printf("biClrUsed: %lu\n", getLongFrom4Bytes(data[46], data[47], data[48], data[49]));
    printf("biClrImportant: %lu\n", getLongFrom4Bytes(data[50], data[51], data[52], data[53]));
}

PUBLIC unsigned long getLongFrom4Bytes(byte by1, byte by2, byte by3, byte by4) {
    unsigned long retval = (unsigned long) by4 << 24 | (unsigned long) by3 << 16;
    retval |= (unsigned long) by2 << 8 | by1;
    return retval;
}

PUBLIC unsigned long getLongFrom2Bytes(byte by1, byte by2) {
    unsigned long retval = (unsigned long) by2 << 8;
    retval |= by1;
    return retval;
}

#endif