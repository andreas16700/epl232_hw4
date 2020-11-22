//
// Created by Andreas Loizides on 18/11/2020.
//

#include <stdlib.h>
#include <string.h>
#include "bmplib.h"
#include "Shared.h"
#define DEBUG_MERGE 0
//this file implements the following functions
//newImage encodeImageWithinImage(FILE* shellImage, FILE* hiddenImage, int bitsToUse);
//newImage decodeHiddenImageFromEncodedImage(FILE* encryptedImage, int bitsToUse);




PRIVATE byte mergeBytes(byte importantByte, byte lessImportantByte, int bitsToUse) {
    if (bitsToUse>7)
        return lessImportantByte;
    if (bitsToUse<1)
        return importantByte;
    byte mask = (byte) 0;
    mask = ~mask;
    //mask now is 8 consecutive ones
    byte upperMask = mask >> (byte) bitsToUse;
    upperMask <<= (byte) bitsToUse;
    byte lowerMask = ~upperMask;

    byte result = (byte) 0;
    byte upperPart = upperMask & importantByte;
    byte lowerPart = lowerMask & lessImportantByte;
    result |= upperPart;
    result |= lowerPart;
    return result;
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
PRIVATE void doneWithFile(FILE* file){
    fflush(file);
    fclose(file);
}

PUBLIC void encodeImageWithinImage(const char *shellImageName,const char *hiddenImageName, int bitsToUse){
    FILE* shellImage = fopen(shellImageName,"rb");
    FILE* hiddenImage = fopen(hiddenImageName,"rb");
    ensureIsValidBMP(shellImage);
    ensureIsValidBMP(hiddenImage);
    char* newName = addPrefix(shellImageName,"new-");
    FILE* output = fopen(newName, "wb");
    ensureFileOpenedForWriting(output,newName);
    copyHeader(hiddenImage,output);
    copyHeader(shellImage,output);
    writeMergedImageData(shellImage,hiddenImage,output,bitsToUse);
    printf("Saved new image as \"%s\"\n",newName);
    doneWithFile(shellImage);
    doneWithFile(hiddenImage);
    doneWithFile(output);
}

PUBLIC void decodeHiddenImageFromEncodedImage(const char* fileNameOfImageWithHiddenImage, int bitsToUse){
    FILE* imageWithHiddenImage = fopen(fileNameOfImageWithHiddenImage, "rb");
    ensureIsValidBMP(imageWithHiddenImage);
    char* newName = addPrefix(fileNameOfImageWithHiddenImage,"new-");
    FILE* extractedImage = fopen(newName,"wb");
    ensureFileOpenedForWriting(extractedImage,newName);
    copyHeader(imageWithHiddenImage,extractedImage);
    extractImageData(imageWithHiddenImage,extractedImage,bitsToUse);
    printf("Saved extracted image as\"%s\"\n",newName);
    doneWithFile(imageWithHiddenImage);
    doneWithFile(extractedImage);
}

#ifdef ENCODEIMAGE_DEBUG
#include <time.h>
#include <assert.h>

void testMergeByte(){
    srand48(time(NULL));
    byte b1 = (unsigned)lrand48()&(unsigned)255;
    byte b2 = (unsigned)lrand48()&(unsigned)255;
    int bitsToUse = (signed)((unsigned) lrand48()&(unsigned)7);
    int diff = 8-bitsToUse;
//    byte lsb

    printf("Visual test:\n");
    printf("\t\t\t\t\t\tdec\tbinary\n");
    for(int i = 0; i<=8; i++) {
        printf("Bits used: %d",i);
        printf("\nImportant byte\t\t:\t%d\t", b1);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b1));
        printf("\nLess important byte\t:\t%d\t", b2);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b2));
        byte result = mergeBytes(b1,b2,i);
        printf("\nResult\t\t\t\t:\t%d\t", result);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(result));
        printf("\n\n");
        fflush(stdout);
    }
}
int main(){
    testMergeByte();
}
#endif