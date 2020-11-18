#include "bmplib.h"

byte *readImage(FILE *imageFile);

void hello(void) {
    printf("Hello, World!\n");
}

byte *readImage(FILE *imageFile) {
    if (imageFile == NULL) {
        printf("Cant read bmp file!\n");
        return 0;
    }
    byte b;
    int index = 0;
    int sizeOfArray = 54;
    byte *data = (byte *) malloc(sizeOfArray * sizeof(byte));
    do {
        b = fgetc(imageFile);
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
    } while (b != EOF);
    return data;
}
