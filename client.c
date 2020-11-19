//
// Created by Andreas Loizides on 18/11/2020.
//

#include "bmplib.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
    //printf("hello ");
    getMetaInfo("tux-pirate.bmp");
    encodeTextInsideAnImage("tux-pirate.bmp", "input.txt", 5);
    printf("hi\n");
    //char *txt = decodeTextFromImage("new-tux-pirate.bmp",5,112);
    //printf("%s\n",txt);
}