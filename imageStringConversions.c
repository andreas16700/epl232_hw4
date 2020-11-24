//
// Created by Andreas Loizides on 20/11/2020.
//
#define DOWN_TO_TOP 1
#include "Shared.h"
#include "bmplib.h"
#include <ctype.h>

PRIVATE byte getBit(String *text, int row, int col, int height) {
    int totalBitIndex = col*height+row;
    int charIndex = totalBitIndex/8;
    int bitIndex = totalBitIndex%8;
    //for invalid index write 0
    if (charIndex>=text->currentCharCount)
        return 0;
    byte character = text->startAddress[charIndex];
    character>>=(unsigned)(7-bitIndex);
    //the wanted bit is now the LSB bit.
    character&=(unsigned)1;
    return character;
}

PRIVATE void convertAndWriteTextAsImageData(String* text, FILE* image, Dimensions imageDimensions){
    int height = imageDimensions.biHeight;
    int width = imageDimensions.biWidth;
    int paddingBytes=4-((imageDimensions.biWidth*3)%4);
    int needsPadding = paddingBytes!=4 ? 1 : 0;
#if DOWN_TO_TOP
    for (int row = 0; row <imageDimensions.biHeight ; row++) {
#else
    for (int row imageDimensions.biHeight-1; row >= 0 ; row--) {
#endif

        for (int col = 0; col < width; ++col) {

            byte bit = getBit(text, row, col, height);
            byte valueToWrite = bit*128;
            for (int i = 0; i < 3; ++i)
                fputc(valueToWrite,image);
        }
        if (needsPadding)
            for (int i = 0; i < paddingBytes; ++i)
                    fputc((byte)0,image);


    }

}
PUBLIC void saveTextAsImage(const char *textFileName, const char *sampleImageName) {
    String* text = readTextFile(textFileName);
    FILE* sample = fopen(sampleImageName,"rb");
    ensureFileExists(sample,sampleImageName);
    char* outputFileName = addPrefix(sampleImageName,"new-");
    FILE* output = fopen(outputFileName,"wb");
    ensureFileOpenedForWriting(output,outputFileName);
    ensureIsValidBMP(sample);
    ensureNotNull(output);
    copyHeader(sample,output);
    convertAndWriteTextAsImageData(text,output,readDimensionsOfImage(sample));
    printf("Saved image as \"%s\"\n",outputFileName);
    free(outputFileName);
    destroyString(text);
    fflush(output);
    fclose(output);
    fclose(sample);
}

typedef struct PIXEL {
    byte r,g,b;
}pixel;
PRIVATE int pixelToASCIIBit(pixel* p){
    static byte oneValue = (byte) 128, zeroValue = (byte) 0;
    byte value = p->r;
    //ensure all bytes are the same value
    if(p->g!=value || p->b!=value){printf("pixel does not have same value for all colors!\n");exit(-5);}

    if (value==oneValue)
        return 1;
    if (value==zeroValue)
        return 0;
    printf("pixel has value %d but expected %d or %d!\n",value,oneValue,zeroValue);
    exit(-5);

}
PRIVATE void printchar(unsigned char theChar) {

    switch (theChar) {

        case '\n':
            printf("%4s","\\n");
            break;
        case '\r':
            printf("%4s","\\r");
            break;
        case '\t':
            printf("%4s","\\t");
            break;
        default:
            if ((theChar < 0x20) || (theChar > 0x7f)) {
                printf("\\%04o", (unsigned char)theChar);
            } else {
                printf("%4c", theChar);
            }
            break;
    }
}

__unused PRIVATE void printCharsInPixelArray(pixel** pixels, Dimensions dimensions){
    char** char2d = malloc(dimensions.biHeight*sizeof(char*));
    for (int i = 0; i < dimensions.biHeight; ++i)
        char2d[i]=malloc(dimensions.biWidth*sizeof(char));
    byte currentAscii=0;
    int readBits=0;
    for (int c = 0; c < dimensions.biWidth; ++c)
//        for (int r = 0; r<dimensions.biHeight; ++r){
        for (int r = dimensions.biHeight-1; r >= 0; --r){
            currentAscii<<=(byte)1;
            currentAscii|=(byte)pixelToASCIIBit(&pixels[r][c]);
            readBits++;
            if (readBits==8){
                char2d[r][c]=currentAscii;
                readBits=0;
                currentAscii=0;
            }else{
                char2d[r][c]=' ';
            }

        }
    printf("\t");
    for (int c = 0; c < dimensions.biWidth; ++c)
        printf("%4d",c);
    printf("\n");
    for (int r = 0; r < dimensions.biHeight; ++r){
        printf("%d\t",r);
        for (int c = 0; c < dimensions.biWidth; ++c) {
            char ch = char2d[r][c];
            if (!isalnum(ch)){
                printchar(ch);
            }

//            if (ch=='\n')
//                printf("%4s","nl");
            else
                printf("%4c", ch);
        }
        printf("\n");
    }

}

__unused PRIVATE void printPixelArray(pixel** pixels, Dimensions dimensions){
    printf("\t");
    for (int c = 0; c < dimensions.biWidth; ++c)
        printf("%4d",c);
    printf("\n");
    for (int r = 0; r < dimensions.biHeight; ++r){
        printf("%d\t",r);
        for (int c = 0; c < dimensions.biWidth; ++c)
            printf("%4d", pixelToASCIIBit(&pixels[r][c]) == 1 ? 128 : 0);
        printf("\n");
    }

}
PRIVATE String* pixelsToString(const pixel** pixels, Dimensions dimensions){
    String* string = newString();
    byte currentASCIIByte=0;
    int readBits=0;
    for (int col = 0; col < dimensions.biWidth; ++col) {
#if DOWN_TO_TOP
        for (int row = dimensions.biHeight-1; row >=0 ; --row) {
#else
        for (int row = 0; row <dimensions.biHeight ; ++row) {
#endif
            pixel pixel1 = pixels[row][col];
            byte newBit = pixelToASCIIBit(&pixel1);
            currentASCIIByte<<=(byte)1;
            currentASCIIByte|=newBit;
            readBits++;
            if (readBits==8) {
                addChar(string, currentASCIIByte);
                readBits=0;
                currentASCIIByte=0;
            }
        }
    }
    return string;
}
PRIVATE pixel** to2DPixelArrayWithoutPadding(const byte* allBytes, Dimensions imageDimensions){
    pixel** pixels = malloc(imageDimensions.biHeight*sizeof(pixel*));
    for (int i = 0; i < imageDimensions.biHeight; ++i)
        pixels[i]=malloc(imageDimensions.biWidth*sizeof(pixel));
    int height=imageDimensions.biHeight,width=imageDimensions.biWidth;
    int paddingBytes=4-((imageDimensions.biWidth*3)%4);
    int needsPadding = paddingBytes!=4 ? 1 : 0;
    int i=0;
    for (int row = height-1; row >= 0 ; row--) {
        for (int col = 0; col < width; ++col){
            byte red = allBytes[i++];
            byte green = allBytes[i++];
            byte blue = allBytes[i++];
            pixel p = {.r=red, .g=green, .b=blue};
            pixels[row][col] = p;
        }
        if (needsPadding)
                i+=paddingBytes;//ignore padding bytes
    }
    return pixels;
}
PRIVATE void free2DPixelArray(pixel** pixels, Dimensions dimensions){
    for (int r = 0; r < dimensions.biHeight; ++r)
        free(pixels[r]);
    free(pixels);
}
PUBLIC void textFromImage(char *imageFileName) {
    FILE* weirdImage = fopen(imageFileName,"rb");
    ensureFileExists(weirdImage,imageFileName);
    ensureIsValidBMP(weirdImage);
    Dimensions dimensions = readDimensionsOfImage(weirdImage);
    byte* rawImageData = readOnlyImageData(weirdImage);
    fclose(weirdImage);
    pixel** pixels = to2DPixelArrayWithoutPadding(rawImageData,dimensions);
    free(rawImageData);
    String* string = pixelsToString((const pixel **) pixels, dimensions);
    free2DPixelArray(pixels,dimensions);
    char* outputName = "outputText.txt";
    saveStringAsTextFile(string,outputName);
    printf("Wrote text in \"%s\"\n",outputName);
    destroyString(string);
}

#ifdef IMAGE_STRING_DEBUG
void debugTextFromImage(char *imageFileName) {
    FILE* weirdImage = fopen(imageFileName,"rb");
    ensureFileExists(weirdImage,imageFileName);
    ensureIsValidBMP(weirdImage);
    Dimensions dimensions = readDimensionsOfImage(weirdImage);
    byte* rawImageData = readOnlyImageData(weirdImage);
    fclose(weirdImage);
    pixel** pixels = to2DPixelArrayWithoutPadding(rawImageData,dimensions);
//    printPixelArray(pixels,dimensions);
    printCharsInPixelArray(pixels,dimensions);
    free(rawImageData);
    String* string = pixelsToString((const pixel **) pixels, dimensions);
    free2DPixelArray(pixels,dimensions);
    char* outputName = "outputText.txt";
    saveStringAsTextFile(string,outputName);
    printf("Wrote text in \"%s\"\n",outputName);
    destroyString(string);
}
void testGetBit(){
    printf("t:\t");
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY('t'));
    printf("\ne:\t");
    printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY('e'));
    printf("\n");
    String* str = newStringFromCharArray("test");
    int height = 3, width=5;
    for (int row = height-1; row >= 0 ; row--) {
        for (int col = 0; col < width; ++col) {
            byte bit = getBit(str, row, col, height);
            byte byteToWrite = bit*128;
            printf("would write ");
            printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(byteToWrite));
            printf(" from row=%d,col=%d\n",row,col);
        }
    }
}
int main(){
//    String* textFile = readTextFile("strFile.txt");
//    saveTextAsImage(textFile,"tux-pirate.bmp","outputImage.bmp");
//    destroyString(textFile);
//    textFromImage("outputImage.bmp","recoveredtp.txt");
debugTextFromImage("pyrros.bmp");
}
#endif