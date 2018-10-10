#ifndef UNTITLED2_ENC_H
#define UNTITLED2_ENC_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

#define MEOW
#ifdef MEOW
#define PRINTF printf
#else
#define PRINTF
#endif

//----------------------------------------------------------------------------------------------------------------------
const char way_to_in[] = {"C:\\Users\\max_l\\CLionProjects\\untitled2\\Hamlet.txt"};
const char way_to_Encyclopedia[] = {"C:\\Users\\max_l\\CLionProjects\\untitled2\\Encyclopedia.txt"};
const char way_to_rhyme[] = {"C:\\Users\\max_l\\CLionProjects\\untitled2\\Rhyme.txt"};
const char way_to_copy[] = {"C:\\Users\\max_l\\CLionProjects\\untitled2\\Hamlet_copy.txt"};
//----------------------------------------------------------------------------------------------------------------------

struct mystring
{
    char* String;
    size_t len;
};

mystring* create_array_of_strings_fromFile(int inFile);
char* fillBuff(int inFile);
long int calcSize(const char* inFile);
size_t calc_number_of_special_symbol(char* buff, char symbol);
mystring* fillAddress(char* buff);
mystring* encyclopediaSorter_from_start(mystring* addressesStrings, size_t numStrings = 0);
int compare_from_start(const void* firstStruct, const void* secondStruct);
void printText(FILE* outFile, mystring* addressesStrings, size_t numStrings = 0);
void destroyer(char* buff, mystring* addressesStrings);
size_t calc_no_zero_Addresses(mystring* addressesStrings);
int compare_from_end(const void* firstStruct, const void* secondStruct);
mystring* encyclopediaSorter_from_end(mystring* addressesStrings, size_t numStrings = 0);

#endif //UNTITLED2_ENC_H
