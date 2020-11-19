//
// Created by Andreas Loizides on 18/11/2020.
//

#include <stdlib.h>
#include <string.h>
#include "bmplib.h"
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
#define DEBUG_MERGE 0
#define HEADER_BYTE_LENGTH 54
//this file implements the following functions
//newImage encodeImageWithinImage(FILE* shellImage, FILE* hiddenImage, int bitsToUse);
//newImage decodeHiddenImageFromEncodedImage(FILE* encryptedImage, int bitsToUse);

PRIVATE void ensureNotNull(void* pntr){
    if (pntr==NULL)
        exit(-1);
}


PRIVATE byte mergeBytes(byte importantByte, byte lessImportantByte, int bitsToUse) {
    if (bitsToUse > 7 || bitsToUse < 1)
        exit(-1);

    byte mask = (byte) 0;
    mask = ~mask;
    //mask now is 8 consecutive zeroes
    byte upperMask = mask >> (byte) bitsToUse;
    upperMask <<= (byte) bitsToUse;
    byte lowerMask = ~upperMask;

    byte result = (byte) 0;
    byte upperPart = upperMask & importantByte;
    byte lowerPart = lowerMask & lessImportantByte;
    result |= upperPart;
    result |= lowerPart;
    if (DEBUG_MERGE) {
    printf("\nUpper mask:\t");
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(upperMask));
    printf("\nLower mask:\t");
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(lowerMask));
    printf("\nImportant byte:\t%d\t", importantByte);
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(importantByte));
    printf("\nLess important byte:\t%d\t", lessImportantByte);
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(lessImportantByte));
    printf("\nResult:\t%d\t", result);
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(result));
    fflush(stdout);
    }
    return result;
}
PRIVATE void copyHeader(FILE* from, FILE* to){
    rewind(from);
    rewind(to);
    for (int i = 0; i < HEADER_BYTE_LENGTH; ++i) {
        byte headerByte = fgetc(from);
        fputc(headerByte,to);
    }
}
PRIVATE byte extractHiddenImageByte(byte mergedByte, int bitsToUse){
    if (bitsToUse>7 || bitsToUse<1)
        exit(-1);
    byte numberOfBitsToDisRegard = 8-bitsToUse;
    byte result = mergedByte<<numberOfBitsToDisRegard;

    if (DEBUG_MERGE) {
        printf("\nMerged byte:\t%d\t", mergedByte);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mergedByte));
        printf("\nResult:\t%d\t", result);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(result));
        fflush(stdout);
    }
    return result;
}
PRIVATE void extractImageData(FILE* srcImage, FILE* extracted, int bitsToUse){
    int signedByte = fgetc(srcImage);
    byte sourceByte = (byte) signedByte;
    while (signedByte!=EOF){
        byte hiddenImageByte = extractHiddenImageByte(sourceByte,bitsToUse);
        fputc(hiddenImageByte,extracted);

        signedByte = fgetc(srcImage);
        sourceByte = (byte) signedByte;
    }
}
PRIVATE void writeMergedImageData(FILE* shellImage
                                  ,FILE* secretImage
                                  ,FILE* outputFile
                                  ,int bitsToUse){
    int signedByte = fgetc(shellImage);
    byte importantByte = (byte) signedByte;
    byte lessImportantByte = fgetc(secretImage);
    while (signedByte != EOF){
        byte mergedByte = mergeBytes(importantByte,lessImportantByte,bitsToUse);
        fputc(mergedByte,outputFile);
        //get the next bytes
        signedByte = fgetc(shellImage);
        importantByte = (byte) signedByte;
        lessImportantByte = fgetc(secretImage);
    }
}
//TODO do bmc checks
int isValidBMPImage(FILE* file){
    ensureNotNull(file);
    //other checks here
    return 1;
}
PRIVATE void appendBeforeExtension(char* slot, char* existing, char* toAppend){
    slot[0]='\0';
    int existingLength = strlen(existing);
    int appendLength = strlen(toAppend);

    int extensionLength = strlen(".xxx");
    strcat(slot,existing);
    slot[existingLength-extensionLength]='\0';
    strcat(slot,toAppend);
    int i = existingLength-extensionLength+appendLength;
    int j;
    for (j = 0; j < extensionLength; ++j) {
        slot[i+j]=existing[existingLength-extensionLength+j];
    }
    slot[i+j]='\0';
}
PUBLIC FILE * encodeImageWithinImage(char *shellImageName, char *hiddenImageName, int bitsToUse){
    FILE* shellImage = fopen(shellImageName,"r");
    FILE* hiddenImage = fopen(hiddenImageName,"r");
    if (!(
            isValidBMPImage(shellImage)
            &&
            isValidBMPImage(hiddenImage)
            ))
        exit(-1);
    char newName[strlen(shellImageName)+5];
    appendBeforeExtension(newName,shellImageName,"-new");

    FILE* output = fopen(newName, "w");
    copyHeader(hiddenImage,output);
    copyHeader(shellImage,output);
    writeMergedImageData(shellImage,hiddenImage,output,bitsToUse);
    fflush(output);
    fclose(shellImage);
    fclose(hiddenImage);
    rewind(output);
    return output;
}
FILE* decodeHiddenImageFromEncodedImage(char* fileNameOfImageWithHiddenImage, int bitsToUse){
    FILE* imageWithHiddenImage = fopen(fileNameOfImageWithHiddenImage, "r");
    if (!isValidBMPImage(imageWithHiddenImage))
        exit(-1);
    char newName[strlen(fileNameOfImageWithHiddenImage)+5];
    appendBeforeExtension(newName,fileNameOfImageWithHiddenImage,"-new");
    FILE* extractedImage = fopen(newName,"w");
    copyHeader(imageWithHiddenImage,extractedImage);
    extractImageData(imageWithHiddenImage,extractedImage,bitsToUse);
    fflush(extractedImage);
    fclose(imageWithHiddenImage);
    rewind(extractedImage);
    return extractedImage;
}

#ifdef ENCODEIMAGE_DEBUG
#define ENCODEIMAGE_DEBUG
int main(){
    //test here
}
#endif