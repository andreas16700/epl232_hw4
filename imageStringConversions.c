//
// Created by Andreas Loizides on 20/11/2020.
//
#include "Shared.h"
#include "bmplib.h"
byte getBit(String* text
            ,int row, int col
            , int height, int width){

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
byte bitToPixel(byte bitFromText){
    return 128*bitFromText;
}
void writePixel(byte* rgb, FILE* image){
    for (int i = 0; i < 3; ++i)
        fputc(rgb[i],image);

}

void convertAndWriteTextAsImageData(String* text, FILE* image, Dimensions imageDimensions){
    int height = imageDimensions.biHeight;
    int width = imageDimensions.biWidth;
//    int totalPixels = width*height;
//    int totalWritten=0;
    int paddingBytes=4-((imageDimensions.biWidth*3)%4);
    int needsPadding = paddingBytes>0 ? 1 : 0;
    for (int row = height-1; row >= 0 ; row--) {
        for (int col = 0; col < width; ++col) {

            byte bit = getBit(text, row, col, height, width);
            byte valueToWrite = bit*128;
            for (int i = 0; i < 3; ++i)
                fputc(valueToWrite,image);
//            totalWritten++;
        }
        if (needsPadding)
            for (int i = 0; i < paddingBytes; ++i)
//                for (int j = 0; j < 3; ++j)
                    fputc((byte)0,image);


    }
//    if (totalWritten<totalPixels)
//        for (int i = 0; i < totalPixels-totalWritten; ++i)
//             fputc(0,image);

}
void saveTextAsImage(String* text, char* sampleImageName, char* outputFileName){
    FILE* sample = fopen(sampleImageName,"rb");
    FILE* output = fopen(outputFileName,"wb");
    ensureIsValidBMP(sample);
    ensureNotNull(output);
    copyHeader(sample,output);
    convertAndWriteTextAsImageData(text,output,readDimensionsOfImage(sample));
    fflush(output);
    fclose(output);
    fclose(sample);
}

typedef struct PIXEL {
    byte r,g,b;
}pixel;
int pixelToASCIIBit(pixel* p){
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
void printCharsInPixelArray(pixel** pixels, Dimensions dimensions){
    char** char2d = malloc(dimensions.biHeight*sizeof(char*));
    for (int i = 0; i < dimensions.biHeight; ++i)
        char2d[i]=malloc(dimensions.biWidth*sizeof(char));
    byte currentAscii=0;
    int readBits=0;
    for (int c = 0; c < dimensions.biWidth; ++c)
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
            if (ch=='\n')
                printf("%4s","nl");
            else
                printf("%4c", ch);
        }
        printf("\n");
    }

}
void printPixelArray(pixel** pixels, Dimensions dimensions){
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
String* pixelsToString(const pixel** pixels, Dimensions dimensions){
    String* string = newString();
    byte currentASCIIByte=0;
    int readBits=0;
    for (int col = 0; col < dimensions.biWidth; ++col) {
        for (int row = dimensions.biHeight-1; row >=0 ; --row) {
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
pixel** to2DPixelArrayWithoutPadding(const byte* allBytes, Dimensions imageDimensions){
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
void free2DPixelArray(pixel** pixels, Dimensions dimensions){
    for (int r = 0; r < dimensions.biHeight; ++r)
        free(pixels[r]);
    free(pixels);
}
void textFromImage(char* imageFileName, char* outputFileName){
    FILE* weirdImage = fopen(imageFileName,"rb");
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
    saveStringAsTextFile(string,outputFileName);
    destroyString(string);
}

#ifdef IMAGE_STRING_DEBUG
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
            byte bit = getBit(str, row, col, height, width);
            byte byteToWrite = bitToPixel(bit);
            printf("would write ");
            printf(BYTE_TO_BINARY_PATTERN,BYTE_TO_BINARY(byteToWrite));
            printf(" from row=%d,col=%d\n",row,col);
        }
    }
}
int main(){
    String* textFile = readTextFile("strFile.txt");
    saveTextAsImage(textFile,"tux-pirate.bmp","outputImage.bmp");
    destroyString(textFile);
    textFromImage("outputImage.bmp","recoveredtp.txt");
}
#endif