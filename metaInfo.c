//
// Created by sohae on 19/11/2020.
//
#ifndef META_INFO
#define META_INFO
#include "bmplib.h"
#include "Shared.h"

PUBLIC unsigned long getLongFrom4Bytes(byte b1, byte b2, byte b3, byte b4);

PUBLIC unsigned long getLongFrom2Bytes(byte b1, byte b2);

void getMetaInfo(char *bmpImage) {
    byte *data = readImage(bmpImage);
    printf("BITMAP_FILE_HEADER\n"
           "==================");
    printf("bfType: %c%c\n", data[0], data[1]);

    printf("bfSize: %lu\n", getLongFrom4Bytes(&data[2]));
    printf("bfReserved1: %u\n", getLongFrom2Bytes(&data[6]));
    printf("bfReserved2: %u\n", getLongFrom2Bytes(&data[8]));
    printf("bfOffBits: %lu\n", getLongFrom4Bytes(&data[10]));
    printf("\n");
    printf("BITMAP_INFO_HEADER\n"
           "==================");
    printf("biSize: %lu\n", getLongFrom4Bytes(&data[14]));
    printf("biWidth: %lu\n", getLongFrom4Bytes(&data[18]));
    printf("biHeight: %lu\n", getLongFrom4Bytes(&data[22]));
    printf("biPlanes: %u\n");
    printf("biBitCount: %u\n");
    printf("biCompression: %lu\n", getLongFrom4Bytes(&data[30]));
    printf("biSizeImage: %lu\n", getLongFrom4Bytes(&data[34]));
    printf("biXPelsPerMeter: %lu\n", getLongFrom4Bytes(&data[38]));
    printf("biYPelsPerMeter: %lu\n", getLongFrom4Bytes(&data[42]));
    printf("biClrUsed: %lu\n", getLongFrom4Bytes(&data[46]));
    printf("biClrImportant: %lu\n", getLongFrom4Bytes(&data[50]));
}

PUBLIC unsigned long getLongFrom4Bytes(byte b1, byte b2, byte b3, byte b4) {
    unsigned long retval = (unsigned long) b4 << 24 | (unsigned long) b3 << 16;
    retval |= (unsigned long) b2 << 8 | b1;
    return retval;
}

PUBLIC unsigned long getLongFrom2Bytes(byte b1, byte b2) {
    unsigned long retval = (unsigned long) b2 << 8;
    retval |= b1;
    return retval;
}

#endif