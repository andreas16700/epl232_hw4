/**
    BMP Image Steganography - Hiding infomation inside BMP images, EPL232 Homework 4, UCY
    Copyright (C) 2020  Sohaib Nassar & Andreas Loizides

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
//
// Created by Andreas Loizides on 18/11/2020.
//

#include "Shared.h"
#include <stdlib.h>
#include "bmplib.h"





PRIVATE byte extractHiddenImageByte(byte mergedByte, int bitsToUse){
    if (bitsToUse>7 || bitsToUse<1)
        exit(-1);
    byte numberOfBitsToDisRegard = 8-bitsToUse;
    byte result = mergedByte<<numberOfBitsToDisRegard;


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
    if (!imagesHaveSameDimensions(shellImage,hiddenImage)){
        fclose(shellImage);fclose(hiddenImage);
        exitWithMessage("The two images must have the same dimensions!");
    }
    char* newName = addPrefix(shellImageName,"new-");
    FILE* output = fopen(newName, "wb");
    ensureFileOpenedForWriting(output,newName);
    copyHeader(hiddenImage,output);
    copyHeader(shellImage,output);
    writeMergedImageData(shellImage,hiddenImage,output,bitsToUse);
    printf("Saved new image as \"%s\"\n",newName);
    free(newName);
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
    free(newName);
    doneWithFile(imageWithHiddenImage);
    doneWithFile(extractedImage);
}

#ifdef ENCODEIMAGE_DEBUG
#include <time.h>
#include <assert.h>

byte onlyMSBAsLSB(byte b, byte bitCount){
    return b>>((unsigned)8-bitCount);
}
void testExtractHiddenImageByte(){
    srand48(time(NULL));

    byte shell = (unsigned)lrand48()&(unsigned)255;
    printf("\nshell:\t%d\t", shell);
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(shell));
    byte hidden = (unsigned)lrand48()&(unsigned)255;
    printf("\nhidden:\t%d\t", shell);
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(hidden));
    for (int i = 1; i < 8; ++i) {
        byte merged = mergeBytes(shell,hidden,i);
        printf("\nmerged:\t%d\t", merged);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(merged));
        byte extracted = extractHiddenImageByte(merged,i);
        printf("\nextracted:\t%d\t", extracted);
        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(extracted));
        byte recovered = onlyMSBAsLSB(extracted,i);
        assert((recovered&hidden)==recovered);
    }

}

void testMergeByte(){
    srand48(time(NULL));
    byte b1 = (unsigned)lrand48()&(unsigned)255;
    byte b2 = (unsigned)lrand48()&(unsigned)255;

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
    testExtractHiddenImageByte();
}
#endif