//
// Created by Andreas Loizides on 24/11/2020.
//
#include "Shared.h"
#include <stdlib.h>
#include "bmplib.h"
#define BITS_TO_USE 4
#define SHUFFLE_COUNT 3

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



typedef struct BYTE_MSB_MODIFICATION {
    byte bit1Index, bit2Index, bitsToFlip;
} ByteMSBModification;
ByteMSBModification *createModificationsArray(byte n, int times) {
    ByteMSBModification* modifications = calloc(times,sizeof(ByteMSBModification));
    ensureNotNull(modifications);
    for (int i = 0; i < times; ++i) {
        modifications[i].bit1Index=(unsigned)lrand48() % (unsigned) n;
        modifications[i].bit2Index=(unsigned)lrand48() % (unsigned) n;
        modifications[i].bitsToFlip=(unsigned)lrand48() % (unsigned) (n+1);
    }

    return modifications;
}
byte applyModificationToNMSB(ByteMSBModification* modification, byte toByte, byte n, byte willDecrypt){
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
byte reorderAndShuffleNMSBMultipleTimes(byte b, byte n, int times, byte willDecrypt){
    ByteMSBModification* modifications = createModificationsArray(n, times);
    int modificationsApplied=0;
    ByteMSBModification* currentModification = willDecrypt ? &modifications[times-1] : &modifications[0];
    byte currentByte=b;
    while (modificationsApplied!=times){
        currentByte = applyModificationToNMSB(currentModification,currentByte,n,willDecrypt);
        modificationsApplied++;
        currentModification = willDecrypt ? currentModification-1 : currentModification+1;
    }
    free(modifications);
    return currentByte;
}

void extractAndDecryptImageData(FILE* srcImage, FILE* extractedImage, const char* password){
    long hash = (signed)calculateHash(password);
    srand48(hash);
    int signedByte = fgetc(srcImage);
    while (signedByte!=EOF){
        byte mergedEncryptedByte = (byte) signedByte;
        byte extractedByte = mergedEncryptedByte<<((unsigned )(8-BITS_TO_USE));
        byte decrypted = reorderAndShuffleNMSBMultipleTimes(extractedByte,BITS_TO_USE,SHUFFLE_COUNT,1);
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
        byte hiddenImageByteShuffled = reorderAndShuffleNMSBMultipleTimes((byte)signedByteHidden,BITS_TO_USE,SHUFFLE_COUNT,0);
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
#include <time.h>
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
void testFlipN(){
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
int main(){
    testsetBit();
    testFlipN();
}
#endif