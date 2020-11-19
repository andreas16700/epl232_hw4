//
// Created by Andreas Loizides on 19/11/2020.
//

#include <stdlib.h>
#include "Shared.h"
void ensureNotNull(void* pntr){
    if (pntr==NULL) {
        printf("Null pointer!\n");
        exit(-1);
    }
}
int isValidBMP(FILE* file){
    //TODO other checks here
    return 1;
}
void ensureIsValidBMP(FILE* image){
    ensureNotNull(image);
    if (!isValidBMP(image)){
        printf("File is not a BMP image.\n");
        exit(-1);
    }

}
char* dropFileExtension(char* ofFileName){
    static const int extensionLength=4;
    int fileNameLength = (int) strlen(ofFileName);
    int indexBeforeExtension = fileNameLength-extensionLength;
    char* extension = malloc(extensionLength+1);
    ensureNotNull(extension);
    strcat(extension, ofFileName+indexBeforeExtension);
    ofFileName[indexBeforeExtension]='\0';
    return extension;
}
char* appendToFileNameBeforeExtension(char* currentFileNameWithExtension, char* stringToAppend){
    int currentNameSize = (int) strlen(currentFileNameWithExtension);
    int appendingStringSize = (int) strlen(stringToAppend);
    int newSize = currentNameSize+appendingStringSize;
    char* newFileName = malloc(newSize+1);
    ensureNotNull(newFileName);
    strcat(newFileName,currentFileNameWithExtension);
    char* extension = dropFileExtension(newFileName);
    strcat(newFileName,stringToAppend);
    strcat(newFileName,extension);
    free(extension);
    return newFileName;
}
unsigned long getLongFrom4Bytes(byte *b) {
    unsigned long retval = (unsigned long) b[3] << 24 | (unsigned long) b[2] << 16;
    retval |= (unsigned long) b[1] << 8 | b[0];
    return retval;
}

unsigned long getLongFrom2Bytes(byte *b) {
    unsigned long retval = (unsigned long) b[2] << 8;
    retval |= b[0];
    return retval;
}