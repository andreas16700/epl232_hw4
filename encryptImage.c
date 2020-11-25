//
// Created by Andreas Loizides on 24/11/2020.
//
#include "Shared.h"
#include <stdlib.h>
#include "bmplib.h"
#include <time.h>
#define BITS_TO_USE 4

unsigned long calculateHash(const char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *(unsigned char*)str++))
        hash = ((hash << (unsigned int)5) + hash) + c; /* hash * 33 + c */

    return hash;
}



byte maskForBit(byte bitIndex){
    return (unsigned)1<<bitIndex;
}
byte setBit(byte ofByte, byte bitIndex, byte bitValue){
    byte result = ofByte;
    //set the bit  at that index to 0
    byte indexMask = maskForBit(bitIndex);
    result&=((byte)~indexMask);
    //bring the bit to the correct index
    byte bitAtCorrectIndex = bitValue << bitIndex;
    result|=bitAtCorrectIndex;
    return result;
}
byte reverseByte(byte ofByte){
    byte result=0;
    for (int i = 0; i < 8; ++i) {
        result<<=(byte)0x1;
        result|=(ofByte&(unsigned)0x1);
        ofByte>>=(byte)0x1;
    }
    return result;
}
byte flipNBits(byte ofByte, byte n){
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
byte shuffleNMostSignificantBits(byte b, int n, byte willDecrypt){
    byte willEncrypt = !willDecrypt;
    if (n<2 || n>8)
        return b;
    byte bitsToIgnore = 8-n;
    byte withOnlyMSB = b>>bitsToIgnore;
    byte bit1Index = (unsigned)lrand48() % (unsigned) n;
    byte bit2Index = (unsigned)lrand48() % (unsigned) n;
    byte bitsToFlip = (unsigned)lrand48() % (unsigned) (n+1);
    while (bit2Index==bit1Index)
        bit2Index = (unsigned)lrand48() % (unsigned) n;
    if (willDecrypt)
        withOnlyMSB=flipNBits(withOnlyMSB,bitsToFlip);
    byte bit1 = (byte)(withOnlyMSB & maskForBit(bit1Index)) >> bit1Index;
    byte bit2 = (byte)(withOnlyMSB & maskForBit(bit2Index)) >> bit2Index;
    byte shuffledMSB = setBit(withOnlyMSB,bit1Index,bit2);
    shuffledMSB=setBit(shuffledMSB,bit2Index,bit1);
    if (willEncrypt)
        shuffledMSB=flipNBits(shuffledMSB,bitsToFlip);
    //make shuffled msb again
    shuffledMSB<<=bitsToIgnore;
    //reset the previous MSB to zero
    byte result = b << (byte)n;
    result>>=(byte)n;
    //combine them
    result|=shuffledMSB;
    return result;
}

void extractAndDecryptImageData(FILE* srcImage, FILE* extractedImage, const char* password){
    long hash = (signed)calculateHash(password);
    srand48(hash);
    int signedByte = fgetc(srcImage);
    while (signedByte!=EOF){
        byte mergedEncryptedByte = (byte) signedByte;
        byte extractedByte = mergedEncryptedByte<<((unsigned )(8-BITS_TO_USE));
        byte decrypted = shuffleNMostSignificantBits(extractedByte,BITS_TO_USE,1);
        fputc(decrypted,extractedImage);

        signedByte = fgetc(srcImage);
    }
}
void encryptAndHideImage(const char* shellImageName, const char* hiddenImageName, const char* password){
    FILE* shellImage = fopen(shellImageName,"rb");
    FILE* hiddenImage = fopen(hiddenImageName,"rb");
    ensureIsValidBMP(shellImage);ensureIsValidBMP(hiddenImage);
    long hash = (signed)calculateHash(password);
    srand48(hash);
    char* encryptedImageName = addPrefix(shellImageName,"encrypted-");
    FILE* encryptedImage = fopen(encryptedImageName,"wb");
    ensureFileOpenedForWriting(encryptedImage,encryptedImageName);
    copyHeader(shellImage,encryptedImage);copyHeader(hiddenImage,encryptedImage);
    int signedByteShell = fgetc(shellImage);
    int signedByteHidden = fgetc(hiddenImage);
    while (signedByteShell!=EOF){
        if (signedByteHidden==EOF)
            exitWithMessage("The two images but have the same dimensions!");
        byte hiddenImageByteShuffled = shuffleNMostSignificantBits((byte)signedByteHidden,BITS_TO_USE,0);
        byte mergedByte = mergeBytes((byte)signedByteShell,hiddenImageByteShuffled,BITS_TO_USE);
        fputc(mergedByte,encryptedImage);
        signedByteShell = fgetc(shellImage);
        signedByteHidden = fgetc(hiddenImage);
    }
    if (signedByteHidden!=EOF)
        exitWithMessage("The two images but have the same dimensions!");
    fclose(encryptedImage);
    fclose(shellImage);
    fclose(hiddenImage);
    printf("Saved image with hidden encrypted image as \"%s\".\nDon't forget your password!\n",encryptedImageName);
    free(encryptedImageName);
}
void decryptHiddenImage(const char* imageWithEncryptedDataName, const char* password){
    //Input (open files)
    FILE* imageWithEncryptedData = fopen(imageWithEncryptedDataName,"rb");
    ensureIsValidBMP(imageWithEncryptedData);
    char* newImageName = addPrefix(imageWithEncryptedDataName,"decrypted-");
    FILE* decryptedImage = fopen(newImageName,"wb");
    //Processing
    copyHeader(imageWithEncryptedData,decryptedImage);
    extractAndDecryptImageData(imageWithEncryptedData,decryptedImage,password);
    //Output
    free(newImageName);
    fclose(imageWithEncryptedData);
    fclose(decryptedImage);
}


#ifdef DEBUG_ENCRYPT_IMAGE
void testsetBit(){
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
void testShuffleBits(){
    srand48(time(NULL));
    byte random = (unsigned)lrand48() & (unsigned)255;
    printf("Byte:%d\n",random);
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(random));printf("\n");
    for (int i = 0; i < 8; ++i) {
        printf("i=%d\n",i);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Original:",BYTE_TO_BINARY(random));
        byte shuffled = shuffleNMostSignificantBits(random,i,1);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Shuffled:",BYTE_TO_BINARY(shuffled));
    }
}
void testFlipN(){
//    srand48(time(NULL));
//    byte random = (unsigned)lrand48() & (unsigned)255;
    byte random = 4;
    printf("Byte:%d\n",random);
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(random));printf("\n");
    for (int i = 0; i < 8; ++i) {
        printf("i=%d\n",i);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Original:",BYTE_TO_BINARY(random));
        byte flipped = flipNBits(random,i);
        printf("%10s\t"BYTE_TO_BINARY_PATTERN"\n","Flipped:",BYTE_TO_BINARY(flipped));
    }
}
int main(){
    testFlipN();
}
#endif