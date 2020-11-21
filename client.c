//
// Created by Andreas Loizides on 18/11/2020.
//

#include "bmplib.h"
#include <stdio.h>
#if !defined(IMAGE_STRING_DEBUG) \
    &&\
    !defined(ENCODEIMAGE_DEBUG)  \
    &&\
    !defined(DEBUG_TEXTENCODINGDECODING)
int main(int argc, char *argv[]) {
    //printf("hello ");
    encodeTextInsideAnImage("IMG_6855.bmp", "input.txt", 5);
}
#endif