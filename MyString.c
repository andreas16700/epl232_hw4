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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MyString.h"
#include "Shared.h"
#include "bmplib.h"

#define STARTING_CAPACITY 4
#define READ_FROMFILE_LINE_CAPACITY 25

PUBLIC String* newStringWithCapacity(int capacity){
    String* new = malloc(sizeof(String));
    new->currentCharCount=0;
    ensureNotNull(new);
    char* charArrayBase = malloc(sizeof(char)*capacity);
    ensureNotNull(charArrayBase);
    new->capacity=capacity;
    new->startAddress=charArrayBase;
    new->startAddress[new->currentCharCount]='\0';
    return new;
}
PUBLIC String* newString(){
    return newStringWithCapacity(STARTING_CAPACITY);
}
PUBLIC String* newStringFromCharArray(const char* array){
    int capacity = (int) strlen(array);
    String* string = newStringWithCapacity(capacity+1);
    addStrCharArray(string,array);
    return string;
}

PRIVATE void setNewStartAddress(String* string, char* newStart){
    string->startAddress=newStart;
}
PRIVATE void makeBigger(String *string){
    string->capacity*=2;
    char* new;
    new = realloc(string->startAddress,string->capacity);
    ensureNotNull(new);
    setNewStartAddress(string, new);
}
PRIVATE void ensureCapacity(String* string, int forNewCharCount){
    while(forNewCharCount+1>=string->capacity)
        makeBigger(string);

}
PRIVATE void ensureEnoughCapacityForNextCharAndNull(String *forString){
    if (forString->currentCharCount+1>=forString->capacity)
        makeBigger(forString);
}
PRIVATE char* nextAddress(String *ofString){
    ensureEnoughCapacityForNextCharAndNull(ofString);
    return &ofString->startAddress[ofString->currentCharCount];
}
PUBLIC void addChar(String *string, char thing){
    char *next = nextAddress(string);
    *next=thing;
    *(next+1)='\0';
    string->currentCharCount++;
}
PUBLIC void destroyString(String* string){
    free(string->startAddress);
    free(string);
}
PRIVATE void ensureCapacityToAddString(String* destination, const String* new){
    int newCapacity = destination->currentCharCount+new->currentCharCount+1;
    ensureCapacity(destination,newCapacity);
}

__unused PUBLIC void addString(String* destination, const String* new){
    ensureCapacityToAddString(destination,new);
    char* newBase = new->startAddress;
    for (int i = 0; i < new->currentCharCount; ++i)
        addChar(destination,newBase[i]);

}

__unused PUBLIC void debugPrint(String* string){
    printf("String \"%s\":\n",string->startAddress);
    printf("Capacity: %d\n",string->capacity);
    printf("%5c",'i');
    int endIndex = string->capacity;
    for (int i = 0; i < endIndex; ++i)
        printf("%5d",i);
    printf("\n%5c",'c');
    for (int i = 0; i < endIndex; ++i) {
        printf("%5c",string->startAddress[i]);
    }
    printf("\n%5c",'d');
    for (int i = 0; i < endIndex; ++i) {
        printf("%5d",string->startAddress[i]);
    }
    printf("\n");
}

__unused PUBLIC void makeEmpty(String* string){
    string->currentCharCount=0;
    string->startAddress[0]='\0';
}
PUBLIC void addStrCharArray(String* string, const char* arr){
    int capacity = (int)strlen(arr);
    for (int i = 0; i < capacity; ++i)
        addChar(string,arr[i]);
}

__unused PUBLIC void trim(String* string){
    int trimmedCapacity=string->currentCharCount+1;
    char* newBase = realloc(string->startAddress,trimmedCapacity);
    ensureNotNull(newBase);
    string->startAddress=newBase;
    string->capacity=trimmedCapacity;
}
PUBLIC String* readTextFile(const char* filename){
    String* fileContents = newString();

    FILE* textFile = fopen(filename,"r");
    char* newLine = malloc(READ_FROMFILE_LINE_CAPACITY);
    ensureFileExists(textFile,filename);
    while (!feof(textFile)){
        fgets(newLine,READ_FROMFILE_LINE_CAPACITY,textFile);
        addStrCharArray(fileContents,newLine);
        newLine[0]='\0';
    }
    fclose(textFile);
    free(newLine);
    return fileContents;
}
PUBLIC void saveStringAsTextFile(String* string, char* fileName){
    FILE* file = fopen(fileName, "w");
    ensureFileOpenedForWriting(file,fileName);
    fputs(string->startAddress,file);
    fflush(file);
    fclose(file);
}