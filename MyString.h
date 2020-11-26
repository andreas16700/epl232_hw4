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
// Created by Andreas Loizides on 21/10/2020.
//

#ifndef SHARED_MYSTRING_H
#define SHARED_MYSTRING_H
#ifndef __unused
#define __unused        __attribute__((__unused__))
#endif

typedef struct {
    int capacity;
    int currentCharCount;
    char *startAddress;
}String;
String* newString();
String* newStringFromCharArray(const char* array);
String* newStringWithCapacity(int capacity);
void addChar(String *string, char thing);
void destroyString(String* string);

__unused void addString(String* destination, const String* new);

__unused void debugPrint(String* string);

__unused void makeEmpty(String* string);
void addStrCharArray(String* string, const char* arr);

__unused void trim(String* string);
String* readTextFile(const char* filename);
void saveStringAsTextFile(String* string, char* fileName);
#endif //SHARED_MYSTRING_H
