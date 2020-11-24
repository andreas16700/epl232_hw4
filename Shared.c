//
// Created by Andreas Loizides on 19/11/2020.
//

#include <stdlib.h>
#include "Shared.h"

void ensureNotNull(void *pntr) {
    if (pntr == NULL) {
        printf("Null pointer!\n");
        exit(-1);
    }
}
byte* readHeader(FILE* file){
    rewind(file);
    byte* bytes = calloc(HEADER_BYTE_LENGTH,sizeof(byte));
    ensureNotNull(bytes);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i) {
        int token = fgetc(file);
        if (token==EOF)
            return NULL;
        bytes[i]=(byte) token;
    }
    return bytes;
}
int isValidBMP(FILE* file) {
    byte* header = readHeader(file);
    rewind(file);
    if (header==NULL)
        return 0;
    return
    //image type must be bitmap
    header[0]=='B' && header[1]=='M'
    &&
    //must be a 24bit image
    getLongFrom2Bytes(&header[28])==24
    &&
    //must not be compressed
    getLongFrom4Bytes(&header[30])==0
    ;
}

void ensureIsValidBMP(FILE *image) {
    ensureNotNull(image);
    if (!isValidBMP(image)) {
        printf("File is not a BMP image.\n");
        exit(-1);
    }

}
int containsPath(const char* filename){
    char c;
    while((c=*(filename++))!='\0')
        if (c=='/' || c=='\\')
            return 1;
    return 0;
}
int indexOfFirstCharOfFileNameWithoutPath(const char* path){
    int lastIndex=(int)strlen(path)-1;
    for (; lastIndex>=0; --lastIndex)
        if (path[lastIndex]=='/' || path[lastIndex]=='\\')
            return lastIndex+1;
    return 0;
}
char* prefixSubstring(const char* current, int prefixLength){
    char* prefix = malloc(prefixLength+1);
    ensureNotNull(prefix);
    for (int i = 0; i < prefixLength; ++i)
        prefix[i]=current[i];
    prefix[prefixLength]='\0';
    return prefix;
}
char* postfixSubstring(const char* current, int postfixLength){
    char* postfix = malloc(postfixLength+1);
    int currentLength = (int) strlen(current);
    ensureNotNull(postfix);
    for (int i = 0; i < postfixLength; ++i)
        postfix[i]=current[currentLength-postfixLength+i];
    postfix[postfixLength]='\0';
    return postfix;
}
char* addPrefixAfterPath(const char *current, const char *prefix){
    int currentLength = (int)strlen(current);
    int fileNameStartIndex = indexOfFirstCharOfFileNameWithoutPath(current);
    int pathSize = fileNameStartIndex;
    int fileNameLength=currentLength-pathSize;
    char* path = prefixSubstring(current,pathSize);
    char* fileName = postfixSubstring(current,fileNameLength);

    char* newNameWithPath = calloc(currentLength+strlen(prefix)+1,sizeof(char));
    ensureNotNull(newNameWithPath);
    strcpy(newNameWithPath,path);
    strcat(newNameWithPath,prefix);
    strcat(newNameWithPath,fileName);
    free(path);
    free(fileName);
    return newNameWithPath;
}
char *addPrefix(const char *current, const char *prefix) {
    if (containsPath(current))
        return addPrefixAfterPath(current,prefix);
    int newSize = (int) strlen(current) + (int) strlen(prefix);
    char *newName = (char *) malloc(newSize + 1);
    ensureNotNull(newName);
    strcpy(newName, prefix);
    strcat(newName, current);
    //just to be sure...
    if (newName[newSize] != '\0')
        newName[newSize] = '\0';
    return newName;
}

int intFromNBytes(const byte *bytes, int n) {
    unsigned int num = 0;
    for (int i = n - 1; i >= 0; --i) {
        num <<= (unsigned) 8;
        num |= bytes[i];
    }
    return (signed) num;
}

unsigned int getLongFrom4Bytes(const byte *b) {
    return (unsigned) intFromNBytes(b, 4);
}

unsigned int getLongFrom2Bytes(const byte *b) {
    return (unsigned) intFromNBytes(b, 2);
}

void copyHeader(FILE *from, FILE *to) {
    rewind(from);
    rewind(to);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i) {
        byte headerByte = fgetc(from);
        fputc(headerByte, to);
    }
}

Dimensions readDimensionsOfImage(FILE *image) {
    rewind(image);
    //skip file header
    int fileHeaderLength = 14, infoHeaderBytesToSkip = 4;
    int bytesToSkip = fileHeaderLength + infoHeaderBytesToSkip;
    for (int i = 0; i < bytesToSkip; ++i)
        fgetc(image);
    byte *biWidth = malloc(4 * sizeof(byte));
    byte *biHeight = malloc(4 * sizeof(byte));
    for (int i = 0; i < 4; ++i)
        biWidth[i] = fgetc(image);
    for (int i = 0; i < 4; ++i)
        biHeight[i] = fgetc(image);
    Dimensions dimensions = {.biHeight=intFromNBytes(biHeight, 4), .biWidth=intFromNBytes(biWidth, 4)};
    return dimensions;
}

byte *readOnlyImageData(FILE *image) {
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
    while (readByte != EOF) {
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
    ensureFileExists(fp, imageFile);
    byte b;
    int readByte;
    int index = 0;
    int sizeOfArray = 54;
    byte *data = (byte *) malloc(sizeOfArray * sizeof(byte));
    readByte = fgetc(fp);
    b = (byte) readByte;
    while (readByte != EOF) {

        data[index++] = b;
        if (index >= sizeOfArray) {
            sizeOfArray *= 2;
            byte *temp = (byte *) realloc(data, sizeOfArray * sizeof(byte));
            ensureNotNull(temp);
            data = temp;
        }
        readByte = fgetc(fp);
        b = (byte) readByte;
    }
    fclose(fp);
    return data;
}

void exitWithMessage(const char *message) {
    printf("%s\n", message);
    exit(-1);
}

void createNewImageFile(char *nameOfNewImage, byte *data) {
    unsigned long sizeOfImage = getLongFrom4Bytes(&data[34]);
    FILE *newImage = fopen(nameOfNewImage, "wb");
    ensureFileOpenedForWriting(newImage, nameOfNewImage);
    for (int i = 0; i < sizeOfImage + 54; i++) {
        fputc(data[i], newImage);
    }
    fclose(newImage);
}

void createNewTextFile(char *nameOfNewTextFile, char *text, int length) {
    FILE *newText = fopen(nameOfNewTextFile, "w");
    ensureFileOpenedForWriting(newText, nameOfNewTextFile);
    for (int i = 0; i < length; i++) {
        fputc(text[i], newText);
    }
    fclose(newText);
}
void ensureFileExists(FILE* file, const char* filename){
    if (file==NULL){
        printf("File \"%s\" could not be found!",filename);
        exitWithMessage("");
    }
}
void ensureFileOpenedForWriting(FILE* file, char* filename){
    if (file==NULL){
        printf("File \"%s\" could not be opened for writing!",filename);
        exitWithMessage("");
    }
}