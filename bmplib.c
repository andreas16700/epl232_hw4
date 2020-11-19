#include "bmplib.h"

byte *readImage(char *imageFile);

void hello(void) {
    printf("Hello, World!\n");
}

byte *readImage(char *imageFile) {
    FILE *fp = fopen(imageFile, "rb");
    if (imageFile == NULL) {
        printf("Cant read bmp file!\n");
        return 0;
    }
    int signedB;
    int index = 0;
    int sizeOfArray = 54;
    byte *data = (byte *) malloc(sizeOfArray * sizeof(byte));
    do {
        signedB = fgetc(fp);
        byte b = (byte) signedB;
        data[index] = b;
        index++;
        if (index >= sizeOfArray) {
            byte *temp = (byte *) realloc(data, sizeOfArray * 2);
            sizeOfArray *= 2;
            if (temp == NULL) {
                printf("Cant allocate memory!\n");
                return 0;
            }
            data = temp;
        }
    } while (signedB != EOF);
    return data;
}
