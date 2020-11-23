//
// Created by sohae on 19/11/2020.
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
}