#ifndef BMPLIB_BMPLIB_H
#define BMPLIB_BMPLIB_H
#define PUBLIC
#define PRIVATE static
#include <stdio.h>

typedef struct IMAGERETURNTYPE* newImage;
typedef struct BMPFILEHEADER* bmpFileHeaderType;
typedef struct BMPINFOHEADER* bmpInfoHeaderType;
typedef struct BMPMETA {
    bmpFileHeaderType fileHeader;
    bmpInfoHeaderType infoHeader;
}* bmpMeta;
/**
 * Prints the meta info of the header file of a bitmap image
 * @param bmpImage
 * the image file
 */
bmpMeta getMetaInfo(FILE* bmpImage);
/**
 * Applies the grayscale filter to a bitmap image to a new image file
 * @param ofImage
 * source image
 * @return
 * (new) image with the filter applied
 */
newImage applyGrayscale(FILE* ofImage);
/**
 * Hides an image inside another image.
 * @param shellImage
 * image to store the hidden image
 * @param hiddenImage
 * the image to hide
 * @param bitsToUse
 * Number of bits to use to store the hidden image in each of the source's 8 bits.
 * 4 or less is recommended. More bits means the hidden image will have greater quality
 * at the expense of the source image's quality.
 * (quality referes to color accuracy of the original image)
 * @return
 * the resulting image
 */
newImage encodeImageWithinImage(FILE* shellImage, FILE* hiddenImage, int bitsToUse);
/**
 * Recovers an image that was steganographically hidden inside another image.
 * Has the opposite effect of encodeImageWithinImage().
 * @param encryptedImage
 * image that contains a hidden image
 * @param bitsToUse
 * bits that were used for the secret image
 * @return
 * resulting new image
 */
newImage decodeHiddenImageFromEncodedImage(FILE* encryptedImage, int bitsToUse);
/**
 * Hide some text inside an image
 * @param sourceImage
 * image to hide the text in
 * @param textToHide
 * the text to hide
 * @return
 * resulting new image
 */
newImage encodeTextInsideAnImage(FILE* sourceImage, FILE* textToHide);
/**
 * Find hidden text inside an image.
 * Has the opposite effect of encodeTextInsideAnImage().
 * @param imageWithHiddenText
 * image that text was hidden into
 * @return
 * the hidden text
 */
char** decodeTextFromImage(FILE* imageWithHiddenText);

/**
 * Convert an image into seemingly nonsense text.
 * @param image
 * image to convert
 * @return
 * the resulting text
 */
char** imageToText(FILE* image);

/**
 * Create an image from some text.
 * Reverses imageToText(FILE* image).
 * @param textFile
 * text that contains image data
 * @return
 * the image that was contained
 */
newImage imageFromTextFile(FILE* textFile);



#endif //BMPLIB_BMPLIB_H
