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
// Created by Andreas Loizides on 24/11/2020.
//
#include "Shared.h"
#include <stdlib.h>
#include "bmplib.h"
#define BITS_TO_USE 4
#define SHUFFLE_COUNT 3

PRIVATE unsigned long calculateHash(const char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *(unsigned char*)str++))
        hash = ((hash << (unsigned int)5) + hash) + c; /* hash * 33 + c */

    return hash;
}

PRIVATE byte maskForBit(byte bitIndex){
    return (unsigned)1<<bitIndex;
}
PRIVATE byte setBit(byte ofByte, byte bitIndex, byte bitValue){
    byte result = ofByte;
    //set the bit  at that index to 0
    byte indexMask = maskForBit(bitIndex);
    result&=((byte)~indexMask);
    //bring the bit to the correct index
    byte bitAtCorrectIndex = bitValue << bitIndex;
    result|=bitAtCorrectIndex;
    return result;
}
PRIVATE byte reverseByte(byte ofByte){
    byte result=0;
    for (int i = 0; i < 8; ++i) {
        result<<=(byte)0x1;
        result|=(ofByte&(unsigned)0x1);
        ofByte>>=(byte)0x1;
    }
    return result;
}
PRIVATE byte flipNBits(byte ofByte, byte n){
    byte result=0;
    for (byte i = 0; i <8 ; ++i) {
        byte bit = ofByte&(byte)0x1;
        if (i<n)
            bit=(~bit);
        result<<=(byte)0x1;
        result|=(byte)(bit&(byte)0x1);
        ofByte>>=(byte)0x1;
    }
    return reverseByte(result);
}



typedef struct BYTE_MSB_MODIFICATION {
    byte bit1Index, bit2Index, bitsToFlip;
} ByteMSBModification;
PRIVATE void createModificationsArray(byte n, int times, ByteMSBModification* modifications) {
    ensureNotNull(modifications);
    for (int i = 0; i < times; ++i) {
        modifications[i].bit1Index=(unsigned)lrand48() % (unsigned) n;
        modifications[i].bit2Index=(unsigned)lrand48() % (unsigned) n;
        modifications[i].bitsToFlip=(unsigned)lrand48() % (unsigned) (n+1);
    }
}
PRIVATE byte applyModificationToNMSB(ByteMSBModification* modification, byte toByte, byte n, byte willDecrypt){
    byte willEncrypt = !willDecrypt;
    byte bitsToIgnore = 8-n;
    byte withOnlyMSB = toByte>>bitsToIgnore;
    if (willDecrypt)
        withOnlyMSB=flipNBits(withOnlyMSB,modification->bitsToFlip);
    byte bit1 = (byte)(withOnlyMSB & maskForBit(modification->bit1Index)) >> modification->bit1Index;
    byte bit2 = (byte)(withOnlyMSB & maskForBit(modification->bit2Index)) >> modification->bit2Index;
    byte shuffledMSB = setBit(withOnlyMSB,modification->bit1Index,bit2);
    shuffledMSB=setBit(shuffledMSB,modification->bit2Index,bit1);
    if (willEncrypt)
        shuffledMSB=flipNBits(shuffledMSB,modification->bitsToFlip);
    //make shuffled msb again
    shuffledMSB<<=bitsToIgnore;
    //reset the previous MSB to zero
    byte result = toByte << (byte)n;
    result>>=(byte)n;
    //combine them
    result|=shuffledMSB;
    return result;
}
PRIVATE byte reorderAndShuffleNMSBMultipleTimes(byte b, byte n, int times, byte willDecrypt ,ByteMSBModification* modifications){
    createModificationsArray(n, times, modifications);
    int modificationsApplied=0;
    ByteMSBModification* currentModification = willDecrypt ? &modifications[times-1] : &modifications[0];
    byte currentByte=b;
    while (modificationsApplied!=times){
        currentByte = applyModificationToNMSB(currentModification,currentByte,n,willDecrypt);
        modificationsApplied++;
        currentModification = willDecrypt ? currentModification-1 : currentModification+1;
    }
    return currentByte;
}

PRIVATE void extractAndDecryptImageData(FILE* srcImage, FILE* extractedImage, const char* password){
    //seed randomizer with the password
    long hash = (signed)calculateHash(password);
    srand48(hash);
    //Because the modifications array will have the same length for every reorder/shuffle
    //we need only declare once memory for it
    //and free it after the last reorder/shuffle
    ByteMSBModification* modifications = calloc(SHUFFLE_COUNT,sizeof(ByteMSBModification));
    ensureNotNull(modifications);
    //must save the byte into a signed type because EOF might be negative
    int signedByte = fgetc(srcImage);
    while (signedByte!=EOF){
        byte mergedEncryptedByte = (byte) signedByte;
        //make the LSB significant to reveal the hidden image
        byte extractedByte = mergedEncryptedByte<<((unsigned )(8-BITS_TO_USE));
        //needs decryptions
        byte decrypted = reorderAndShuffleNMSBMultipleTimes(extractedByte,BITS_TO_USE,SHUFFLE_COUNT,1,modifications);
        fputc(decrypted,extractedImage);

        signedByte = fgetc(srcImage);
    }
    free(modifications);
}

PRIVATE void encryptAndWriteImageData(const char *password, FILE *shellImage, FILE *hiddenImage,
                              FILE *encryptedImage) {
    long hash = (signed)calculateHash(password);
    srand48(hash);
    //Because the modifications array will have the same length for every reorder/shuffle
    //we need only declare once memory for it
    //and free it after the last reorder/shuffle
    ByteMSBModification* modifications = calloc(SHUFFLE_COUNT,sizeof(ByteMSBModification));
    ensureNotNull(modifications);
    int signedByteShell = fgetc(shellImage);
    int signedByteHidden = fgetc(hiddenImage);
    while (signedByteShell!=EOF){
        if (signedByteHidden==EOF)
            exitWithMessage("The two images but have the same dimensions!");
        byte hiddenImageByteShuffled = reorderAndShuffleNMSBMultipleTimes((byte)signedByteHidden,BITS_TO_USE,SHUFFLE_COUNT,0,modifications);
        byte mergedByte = mergeBytes((byte)signedByteShell,hiddenImageByteShuffled,BITS_TO_USE);
        fputc(mergedByte,encryptedImage);
        signedByteShell = fgetc(shellImage);
        signedByteHidden = fgetc(hiddenImage);
    }
    free(modifications);
    if (signedByteHidden!=EOF)
        exitWithMessage("The two images but have the same dimensions!");
}

PUBLIC void encryptAndHideImage(const char* shellImageName, const char* hiddenImageName, const char* password){
    //Input (open files)
    FILE* shellImage = fopen(shellImageName,"rb");
    FILE* hiddenImage = fopen(hiddenImageName,"rb");
    ensureIsValidBMP(shellImage);ensureIsValidBMP(hiddenImage);
    if (!imagesHaveSameDimensions(shellImage,hiddenImage)){
        fclose(shellImage);fclose(hiddenImage);
        exitWithMessage("The two images must have the same dimensions");
    }
    char* encryptedImageName = addPrefix(shellImageName,"encrypted-");
    FILE* encryptedImage = fopen(encryptedImageName,"wb");
    ensureFileOpenedForWriting(encryptedImage,encryptedImageName);
    //Processing and Output (merge, encrypt and write bytes)
    copyHeader(shellImage,encryptedImage);copyHeader(hiddenImage,encryptedImage);
    encryptAndWriteImageData(password, shellImage, hiddenImage, encryptedImage);
    fclose(encryptedImage);
    fclose(shellImage);
    fclose(hiddenImage);
    printf("Saved image with hidden encrypted image as \"%s\".\nDon't forget your password!\n",encryptedImageName);
    free(encryptedImageName);
}
PUBLIC void decryptHiddenImage(const char* imageWithEncryptedDataName, const char* password){
    //Input (open files)
    FILE* imageWithEncryptedData = fopen(imageWithEncryptedDataName,"rb");
    ensureIsValidBMP(imageWithEncryptedData);
    char* newImageName = addPrefix(imageWithEncryptedDataName,"decrypted-");
    FILE* decryptedImage = fopen(newImageName,"wb");
    //Processing & Output
    copyHeader(imageWithEncryptedData,decryptedImage);
    extractAndDecryptImageData(imageWithEncryptedData,decryptedImage,password);
    free(newImageName);
    fclose(imageWithEncryptedData);
    fclose(decryptedImage);
}


#ifdef DEBUG_ENCRYPT_IMAGE
#include <time.h>
void testsetBit(){
    printf("setBit(): set a bit at a specified index to a specified value.\n");
    srand48(time(NULL));
    byte random = (unsigned)lrand48() & (unsigned)255;
    printf("Byte:%d\n",random);
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(random));printf("\n");
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 8; ++j) {
            byte result = setBit(random,j,i);
            printf("\nSet bit at index %d to %d\t",j,i);
            printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(result));
        }
    }
}
void testFlipN(){
    printf("flipN(): flip the first N bits in a byte.\n");
    srand48(time(NULL));
    byte random = (unsigned)lrand48() & (unsigned)255;
    printf("Byte:%d\n",random);
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(random));printf("\n");
    for (int i = 0; i < 8; ++i) {
        printf("i=%d\n",i);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Original:",BYTE_TO_BINARY(random));
        byte flipped = flipNBits(random,i);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Flipped:",BYTE_TO_BINARY(flipped));
    }
}
void test(){
    testsetBit();
    testFlipN();
}
void printUsage(){
    printf("Correct usage:\n-e imgShell.bmp imgToHide.bmp passwordhere\n-d shelWithEncrypted passwordhere\n");
}
int main(int argc, char *argv[]) {
    printf("Hello. Choose an option:\n[1] test methods\n[2] encrypt and hide image inside another image");
    int i;
    do {
        i = getchar();
    }while (i=='1' || i=='2');
    if (i=='1') {
        test();
        return 0;
    }
    if (argc<2) {
        printUsage();
        return -1;
    }
    for (i = 0; i < argc; ++i) {
        if (*argv[i]=='-') {
            if (argv[i][1] == 'e' && (argc - 1 - i) == 3) {
                encryptAndHideImage(argv[i + 1], argv[i + 2], argv[i + 3]);
                return 0;
            } else {
                if (argv[i][1] == 'd' && (argc - 1 - i) == 2)
                    decryptHiddenImage(argv[i + 1], argv[i + 2]);
                return 0;
            }
        }
    }
    printUsage();
    return 0;
}
#endif