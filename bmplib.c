#include "bmplib.h"
#include "Shared.h"


byte* readOnlyImageData(FILE* image){
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
    while(readByte != EOF) {
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
    while(readByte != EOF) {

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
