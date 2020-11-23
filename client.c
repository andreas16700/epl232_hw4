//
// Created by Andreas Loizides on 18/11/2020.
//

#include "bmplib.h"
#include <stdio.h>
#include <stdbool.h>

static char* options[]={
         "-list"
        ,"-grayscale"
        ,"-encodeStegano"
        ,"-decodeStegano"
        ,"-encodeText"
        ,"-decodeText"
        ,"-stringToImage"
        ,"-imageToString"
};

int digittoint(char i);

bool isnumber(char i);

int getFunctionNumber(char* fromOption){
    for (int i = 0; i < sizeof(options); ++i)
        if (strcmp(fromOption,options[i])==0)
            return i;
    return -1;
}
int isValidOption(char* option){
    int optionIndex = getFunctionNumber(option);
    return optionIndex>=0 && optionIndex<(int)sizeof(options);
}
int toNumber(char* str){
    int num=0;
    do{
        num*=10;
        num+=digittoint(*str);
    }while (*(++str)!='\0');
    return num;
}

int digittoint(char i) {
    return i-'0';
}

int isNumber(char* str){
    do{
        if (!isnumber(*str))
            return 0;
    }while (*(++str)!='\0');
    return 1;
}

bool isnumber(char i) {
    return i>= '0' && i<= '9';
}

void choseOption(char* option, char*arguments[], int argumentCount){
    if (!isValidOption(option)){
        printf("\"%s\" is not a valid option!\n",option);
        exit(-1);
    }
    int optionIndex = getFunctionNumber(option);
    switch (optionIndex) {
        case 0:
            for (int i = 0; i < argumentCount; ++i)
                getMetaInfo(arguments[i]);
            break;
        case 1:
            for (int i = 0; i < argumentCount; ++i)
                applyGrayscale(arguments[i]);
            break;
        case 2:
            if(!isnumber(*arguments[0]))
                exitWithMessage("Please specify a number of bits to use. (1-7)");
            if (argumentCount!=3)
                exitWithMessage("Please specify the name of the shell image as well as the image to hide.");
            encodeImageWithinImage(arguments[1],arguments[2],digittoint(*arguments[0]));
            break;
        case 3:
            if(!isnumber(*arguments[0]))
                exitWithMessage("Please specify a number of bits to use. (1-7)");
            if (argumentCount!=2)
                exitWithMessage("Please specify the name of the image.");
            decodeHiddenImageFromEncodedImage(arguments[1],digittoint(*arguments[0]));
            break;
        case 4:
            if (argumentCount!=2)
                exitWithMessage("Please specify an image name and a text file.");
            //TODO key??
            encodeTextInsideAnImage(arguments[0],arguments[1],-5);
            break;
        case 5:
            if (argumentCount!=3)
                exitWithMessage("Please specify the image name, the message's length and a filename to write the hidden text into.");
            if (!isNumber(arguments[1]))
                exitWithMessage("Message length must be a number");
            decodeTextFromImage(arguments[0],arguments[2],-5,toNumber(arguments[1]));
            break;
        case 6:
            if (argumentCount!=2)
                exitWithMessage("Please specify a sample image name and the filename of the text to use.");
            saveTextAsImage(arguments[1],arguments[0]);
            break;
        case 7:
            if (argumentCount!=1)
                exitWithMessage("Please specify just the image name.");
            textFromImage(arguments[0]);
            break;
        default:
            exitWithMessage("Illegal option!");
    }
}
#if !defined(IMAGE_STRING_DEBUG) \
    &&\
    !defined(ENCODEIMAGE_DEBUG)  \
    &&\
    !defined(DEBUG_TEXTENCODINGDECODING)
int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i)
        if (*argv[i]=='-') {
            choseOption(argv[i], argv + i + 1, argc - (i + 1));
            return 0;
        }
    printf("Please specify an option and arguments!\n(e.g. ./bmpSteganography -imageToString image.bmp)\n");
}
#endif