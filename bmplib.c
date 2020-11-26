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
#include "bmplib.h"
#include "Shared.h"

#ifdef BMPLIB_TEST
int imagesAreTheSame(const char* imageName1, const char* imageName2){
    FILE* img1 = fopen(imageName1,"rb");
    FILE* img2 = fopen(imageName2,"rb");
    ensureIsValidBMP(img1);
    ensureIsValidBMP(img2);
    int img1ByteSigned = fgetc(img1);
    int img2ByteSigned = fgetc(img2);
    while (img1ByteSigned!=EOF){
        byte img1Byte = (byte) img1ByteSigned;
        byte img2Byte = (byte) img2ByteSigned;
        if (img1Byte!=img2Byte)
            return 0;
        img2ByteSigned=fgetc(img2);
        img1ByteSigned=fgetc(img1);
    }
    //if the two are the same then when img1 is at EOF img2 should also be at EOF.
    return img2ByteSigned==EOF;
}
void testGrayScale(char* imageName){
    applyGrayscale(imageName);
    char* gray = addPrefix(imageName,"gray-");
    FILE* grayImage = fopen(gray,"rb");
    printf("Ensuring image %s is valid bmp..\n",gray);
    free(gray);
    ensureIsValidBMP(grayImage);
    fclose(grayImage);
}
void testEncodeText(char* imageName, char* textName){
    //TODO
}
static char* imageNames[]={
        "tux-pirate.bmp"
        ,"tux-bonaparte.bmp"
        ,"new-tux-pirate.bmp"
        ,"new-tux-bonaparte.bmp"
        ,"new-new-IMG_6865.bmp"
        ,"new-IMG_6865.bmp"
        ,"IMG_6876.bmp"
        ,"IMG_6875.bmp"
        ,"IMG_6865.bmp"
        ,"IMG_6855.bmp"
        ,"imageErr-8bit.bmp"
        ,"image7.bmp"
        ,"image6.bmp"
        ,"image5.bmp"
        ,"image4.bmp"
        ,"image3.bmp"
        ,"image2.bmp"
        ,"image1.bmp"
        ,"grayscale.bmp"
        ,"4x3.bmp"
        ,"3x4.bmp"
};
static int imagesCount=21;
static char* textFileNames[]={
        "strFile.txt"
        ,"poem.txt"
        ,"input.txt"
};
static int textFilesCount=3;

int main(){

}
#endif