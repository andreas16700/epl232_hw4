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
newImage applyGrayscale(FILE *ofImage);

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
FILE *encodeImageWithinImage(char *shellImageName, char *hiddenImageName, int bitsToUse);

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
void decodeHiddenImageFromEncodedImage(char *fileNameOfImageWithHiddenImage, int bitsToUse);

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
void decodeTextFromImage(char *imageWithHiddenTextFileName, unsigned int key, int length);

/**
 * Convert an image into seemingly nonsense text.
 * @param image
 * image to convert
 * @return
 * the resulting text
 */
char **imageToText(FILE *image);

/**
 * Create an image from some text.
 * Reverses imageToText(FILE* image).
 * @param textFile
 * text that contains image data
 * @return
 * the image that was contained
 */
newImage imageFromTextFile(FILE *textFile);

/**
 * Reads the bmp file from given file.
 * Saves it in an array which it returns
 * @param imageFile
 * image to be read
 * @return
 * the array with the data
 */
byte *readImage(char *imageFile);

#endif //BMPLIB_BMPLIB_H
