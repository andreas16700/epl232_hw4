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
