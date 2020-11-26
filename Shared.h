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
// Created by Andreas Loizides on 19/11/2020.
//
#define _XOPEN_SOURCE
#ifndef BMPLIB_SHARED_H
#define BMPLIB_SHARED_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MyString.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & (unsigned) 0x80 ? '1' : '0'), \
  (byte & (unsigned) 0x40 ? '1' : '0'), \
  (byte & (unsigned) 0x20 ? '1' : '0'), \
  (byte & (unsigned) 0x10 ? '1' : '0'), \
  (byte & (unsigned) 0x08 ? '1' : '0'), \
  (byte & (unsigned) 0x04 ? '1' : '0'), \
  (byte & (unsigned) 0x02 ? '1' : '0'), \
  (byte & (unsigned) 0x01 ? '1' : '0')
#define HEADER_BYTE_LENGTH 54
typedef unsigned char byte;
__unused typedef unsigned short int word;
__unused typedef unsigned int dword;

void ensureIsValidBMP(FILE *image);

void ensureNotNull(void *pntr);

char *addPrefix(const char *current, const char *prefix);

/**
 * returns an int from an array of 4 bytes.
 * Big Endian
 * @param b array of bytes
 * @return the int
 */
unsigned int getLongFrom4Bytes(const byte *b);

/**
 * returns an int from an array of 2 bytes.
 * Big Endian
 * @param b array of bytes
 * @return the int
 */
unsigned int getLongFrom2Bytes(const byte *b);

/**
 * Creates a new image from the given array of bytes.
 * creates a new file with the given name
 * and moves the data from the given array to the file
 * @param
 * data array of bytes
 * @param
 * nameOfNewImage name of new bmp file
 */
void createNewImageFile(char *nameOfNewImage, byte *data);

/**
 * Creates a new text file from and puts the given text in it.
 * creates a new file with the given name
 * @param nameOfNewTextFile
 * the name of the new text file
 * @param text
 * teh text to be printed in the file
 * @param length
 * the length of the text
 */
void createNewTextFile(char *nameOfNewTextFile, char *text, int length);

typedef struct DIMENSIONS {
    int biWidth;
    int biHeight;
} Dimensions;

Dimensions readDimensionsOfImage(FILE *image);

void copyHeader(FILE *from, FILE *to);

/**
 * Reads the bmp file from given file.
 * Saves it in an array which it returns
 * @param imageFile
 * image to be read
 * @return
 * the array with the data
 */
byte *readImage(char *imageFile);

byte *readOnlyImageData(FILE *image);

void exitWithMessage(const char *message);

void ensureFileExists(FILE* file, const char* filename);
void ensureFileOpenedForWriting(FILE* file, char* filename);
byte mergeBytes(byte importantByte, byte lessImportantByte, int bitsToUse);
int imagesHaveSameDimensions(FILE* img1, FILE* img2);

#endif //BMPLIB_SHARED_H
