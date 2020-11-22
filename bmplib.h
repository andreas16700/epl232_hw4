#ifndef BMPLIB_BMPLIB_H
#define BMPLIB_BMPLIB_H
#define PUBLIC
#define PRIVATE static

#include <stdio.h>
#include <stdlib.h>
#include "Shared.h"

typedef struct IMAGERETURNTYPE *newImage;



/**
 * Prints the meta info of the header file of a bitmap image
 * @param bmpImage
 * the image file
 */
void getMetaInfo(char *bmpImage);

/**
 * Applies the grayscale filter to a bitmap image to a new image file
 * @param ofImage
 * source image
 * @return
 * (new) image with the filter applied
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
 * @param sourceImage
 * image to hide the text in
 * @param textToHide
 * the text to hide
 * @param sourceImageName
 * the name of the file that contains the image
 * @param key
 * the systemkey that is going to be used t encrypt the text
 * @return
 * resulting new image
 */
void encodeTextInsideAnImage(char *sourceImageFileName, char *textToHideFileName, int key);

/**
 * Find hidden text inside an image.
 * Has the opposite effect of encodeTextInsideAnImage().
 * @param imageWithHiddenText
 * @param key
 * key to decrypt text
 * image that text was hidden into
 * @return
 * the hidden text
 */
char *decodeTextFromImage(char *imageWithHiddenTextFileName, unsigned int key, int length);

/**
 * Save an image as seemingly nonsense image.
 * @param text
 * text to save as image
 * @param sampleImageName
 * the resulting image will have the same dimensions as this sample image
 * @param outputFileName
 * name to save the resulting image
 */
void saveTextAsImage(String* text, char* sampleImageName, char* outputFileName);

 /**
  * Recover text stored as image.
  * Reverses saveTextAsImage(String* text, char* sampleImageName, char* outputFileName).
  * @param imageFileName
  * image that was converted from text
  * @param outputFileName
  * name of text file to write
  */
void textFromImage(char* imageFileName, char* outputFileName);

/**
 * Reads the bmp file from given file.
 * Saves it in an array which it returns
 * @param imageFile
 * image to be read
 * @return
 * the array with the data
 */
byte *readImage(char *imageFile);
byte* readOnlyImageData(FILE* image);

#endif //BMPLIB_BMPLIB_H
