//
// Created by Andreas Loizides on 21/10/2020.
//

#ifndef SHARED_MYSTRING_H
#define SHARED_MYSTRING_H

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
void addString(String* destination, const String* new);
void debugPrint(String* string);
void makeEmpty(String* string);
void addStrCharArray(String* string, const char* arr);
void trim(String* string);
String* readTextFile(const char* filename);
void saveStringAsTextFile(String* string, char* fileName);
#endif //SHARED_MYSTRING_H
