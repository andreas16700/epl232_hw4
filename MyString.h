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
    /**
     * current capacity allocated at the startAddress
     */
    int capacity;
    /**
     * Count of characters currently in the string (not including null character)
     */
    int currentCharCount;
    /**
     * base address of the char array
     */
    char *startAddress;
}String;
/**
 * Allocates memory for a new string of a preset capacity (4).
 * @return
 * address of the new string
 */
String* newString();
/**
 * Creates a new string by copying a character array
 * @param array
 * character array to copy
 * @return
 * address of the new string
 */
String* newStringFromCharArray(const char* array);
/**
 * Create a new empty string with a specified capacity
 * @param capacity
 * capacity to initialize the char array
 * @return
 * address of the new string
 */
String* newStringWithCapacity(int capacity);
/**
 * Add a new character to a string. Increases (doubles) the capacity if needed.
 * @param string
 * string to modify
 * @param thing
 * character to append to the string
 */
void addChar(String *string, char thing);
/**
 * free all memory related to the string
 * @param string
 * string to despose of
 */
void destroyString(String* string);
/**
 * Append (by copying) a string to an existing string
 * @param destination
 * string to modify
 * @param new
 * string to append
 */
__unused void addString(String* destination, const String* new);
/**
 * Print every character in the allocated char array (even after the null character)
 * @param string
 * string to print
 */
__unused void debugPrint(String* string);
/**
 * Inserts the null character at the beginning of the char array and marks it as empty
 * @param string
 * string to modify
 */
__unused void makeEmpty(String* string);
/**
 * Append a char array (by copying) to an existing string
 * @param string
 * string to modify
 * @param arr
 * array to copy
 */
void addStrCharArray(String* string, const char* arr);
/**
 * Resize the char array so that its length equals the string's length
 * @param string
 * string to modify
 */
__unused void trim(String* string);
/**
 * Read and store the contents of a textfile inside a new string
 * @param filename
 * path/name of the textfile to open.
 * @return
 * address of the new string
 */
String* readTextFile(const char* filename);
/**
 * Save a string inside a textfile
 * @param string
 * string to save
 * @param fileName
 * path/name of the output file. If it already exists it'll overwrite it.
 */
void saveStringAsTextFile(String* string, char* fileName);
#endif //SHARED_MYSTRING_H
