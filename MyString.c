//
// Created by Andreas Loizides on 21/10/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MyString.h"
#include "Shared.h"
#define STARTING_CAPACITY 4
#define READ_FROMFILE_LINE_CAPACITY 25

String* newStringWithCapacity(int capacity){
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
String* newString(){
    return newStringWithCapacity(STARTING_CAPACITY);
}
String* newStringFromCharArray(const char* array){
    int capacity = strlen(array);
    String* string = newStringWithCapacity(capacity+1);
    addStrCharArray(string,array);
    return string;
}

void setNewStartAddress(String* string, char* newStart){
    string->startAddress=newStart;
}
void makeBigger(String *string){
    string->capacity*=2;
    char* new;
    new = realloc(string->startAddress,string->capacity);
    ensureNotNull(new);
    setNewStartAddress(string, new);
}
void ensureCapacity(String* string, int forNewCharCount){
    while(forNewCharCount+1>=string->capacity)
        makeBigger(string);

}
void ensureEnoughCapacityForNextCharAndNull(String *forString){
    if (forString->currentCharCount+1>=forString->capacity)
        makeBigger(forString);
}
char* nextAddress(String *ofString){
    ensureEnoughCapacityForNextCharAndNull(ofString);
    return &ofString->startAddress[ofString->currentCharCount];
}
void addChar(String *string, char thing){
    char *next = nextAddress(string);
    *next=thing;
    *(next+1)='\0';
    string->currentCharCount++;
}
void destroyString(String* string){
    free(string->startAddress);
    free(string);
}
void ensureCapacityToAddString(String* destination, const String* new){
    int newCapacity = destination->currentCharCount+new->currentCharCount+1;
    ensureCapacity(destination,newCapacity);
}
void addString(String* destination, const String* new){
    ensureCapacityToAddString(destination,new);
    char* newBase = new->startAddress;
    for (int i = 0; i < new->currentCharCount; ++i)
        addChar(destination,newBase[i]);

}
void debugPrint(String* string){
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
void makeEmpty(String* string){
    string->currentCharCount=0;
    string->startAddress[0]='\0';
}
void addStrCharArray(String* string, const char* arr){
    int capacity = strlen(arr);
    for (int i = 0; i < capacity; ++i)
        addChar(string,arr[i]);
}
void trim(String* string){
    int trimmedCapacity=string->currentCharCount+1;
    char* newBase = realloc(string->startAddress,trimmedCapacity);
    ensureNotNull(newBase);
    string->startAddress=newBase;
    string->capacity=trimmedCapacity;
}
String* readTextFile(const char* filename){
    String* fileContents = newString();

    FILE* textFile = fopen(filename,"r");
    char* newLine = malloc(READ_FROMFILE_LINE_CAPACITY);
    ensureNotNull(newLine);
    while (!feof(textFile)){
        fgets(newLine,READ_FROMFILE_LINE_CAPACITY,textFile);
        addStrCharArray(fileContents,newLine);
        newLine[0]='\0';
    }
    free(newLine);
    return fileContents;
}
void saveStringAsTextFile(String* string, char* fileName){
    FILE* file = fopen(fileName, "w");
    ensureNotNull(fileName);
    fputs(string->startAddress,file);
    fflush(file);
    fclose(file);
}