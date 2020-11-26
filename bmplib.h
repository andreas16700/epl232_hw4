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
#ifndef BMPLIB_BMPLIB_H
#define BMPLIB_BMPLIB_H
#define PUBLIC
#define PRIVATE static
//#define DEBUG_ENCRYPT_IMAGE

#include <stdio.h>
#include <stdlib.h>
#include "Shared.h"



/**
 * Prints the meta info of the header file of a bitmap image
 * @param bmpImage
 * the image file
 */
void getMetaInfo(char *bmpImage);

/**
 * Applies the grayscale filter to a bitmap image to a new image file
 * @param imageFileName
 * file name of source image
 */
void applyGrayscale(char *imageFileName);

/**
 * Hides an image inside another image.
 * @param shellImageName
 * image to store the hidden image
 * @param hiddenImageName
 * the image to hide
 * @param bitsToUse
 * Number of bits to use to store the hidden image in each of the source's 8 bits.
 * 4 or less is recommended. More bits means the hidden image will have greater quality
 * at the expense of the source image's quality.
 * (quality referes to color accuracy of the original image)
 * @return
 * the resulting image
 */
void encodeImageWithinImage(const char *shellImageName,const char *hiddenImageName, int bitsToUse);

/**
 * Recovers an image that was steganographically hidden inside another image.
 * Has the opposite effect of encodeImageWithinImage().
 * @param fileNameOfImageWithHiddenImage
 * image that contains a hidden image
 * @param bitsToUse
 * bits that were used for the secret image
 * @return
 * resulting new image
 */
void decodeHiddenImageFromEncodedImage(const char *fileNameOfImageWithHiddenImage, int bitsToUse);

/**
 * Hide some text inside an image
 * @param sourceImageFileName
 * image name to hide text into
 * @param textToHideFileName
 * file name of text to be hidden
 * @param key
 * the systemkey that is going to be used t encrypt the text
 */
void encodeTextInsideAnImage(char *sourceImageFileName, char *textToHideFileName, int key);

/**
 * Find hidden text inside an image.
 * Has the opposite effect of encodeTextInsideAnImage().
 * @param imageWithHiddenText
 * file name of image with hidden text
 * @param newFileName
 * name of new file that decoded text will be printed in
 * @param key
 * key to decrypt text
 * @param length
 * length of text to be decoded
 */
void decodeTextFromImage(char *imageWithHiddenText, char *newFileName, int key, int length);

/**
 * Save an image as seemingly nonsense image.
 * @param textFileName
 * text to save as image
 * @param sampleImageName
 * the resulting image will have the same dimensions as this sample image
 * @param outputFileName
 * name to save the resulting image
 */
void saveTextAsImage(const char *textFileName, const char *sampleImageName);

 /**
  * Recover text stored as image.
  * Reverses saveTextAsImage(String* text, char* sampleImageName, char* outputFileName).
  * @param imageFileName
  * image that was converted from text
  * @param outputFileName
  * name of text file to write
  */
 void textFromImage(char *imageFileName);

void encryptAndHideImage(const char* shellImageName, const char* hiddenImageName, const char* password);
void decryptHiddenImage(const char* imageWithEncryptedDataName, const char* password);


#endif //BMPLIB_BMPLIB_H
