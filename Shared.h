//
// Created by Andreas Loizides on 19/11/2020.
//

#ifndef BMPLIB_SHARED_H
#define BMPLIB_SHARED_H
#include <stdio.h>
#include <string.h>
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
typedef unsigned short int word;
typedef unsigned int dword;
void ensureIsValidBMP(FILE* image);
void ensureNotNull(void* pntr);
char* addPrefix(const char* current, const char* prefix);
unsigned long getLongFrom4Bytes(byte *b);
unsigned long getLongFrom2Bytes(byte *b);
typedef struct DIMENSIONS{
    int biWidth;
    int biHeight;
}Dimensions;
Dimensions readDimensionsOfImage(FILE* image);
void copyHeader(FILE* from, FILE* to);
#endif //BMPLIB_SHARED_H
