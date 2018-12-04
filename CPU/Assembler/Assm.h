//
// Created by max_l on 04.11.2018.
//

#ifndef CPU_ASSM_H
#define CPU_ASSM_H

#include <cstdio>
#include <cmath>
#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

//----------------------------------------------------------------------------------------------------------------------
const char way_to_in[] = {"C:\\Users\\max_l\\CLionProjects\\CPU\\source_code.txt"};
const char way_to_tell[] = {"C:\\Users\\max_l\\CLionProjects\\CPU\\tell.txt"};
const int size_of_max_operation = 2;
const int Num_of_labels = 20;
//----------------------------------------------------------------------------------------------------------------------

struct mystring
{
    char* String;
    size_t len;
};

struct LABELS{
    char* word;
    int pos_in_bin_code;
};

//----------------------------------------------------------------------------------------------------------------------

char* Assembler(int source_code, FILE* byte_code);
mystring* create_array_of_strings_fromFile(int inFile);
char* fillBuff(int inFile);
long int calcSize(const char* inFile);
size_t calc_number_of_special_symbol(char* buff, char symbol);
mystring* fillAddress(char* buff);
size_t calc_no_zero_Addresses(mystring* addressesStrings);
void destroyer(char* buff, mystring* addressesStrings);
void Print_bin_code(mystring* Code);
char* compile(mystring* Code, LABELS* labels);
int cut_comment(char* string);
int search_in_labels(char* name_of_label, LABELS* labels, int num_of_last);
int write_comand(char* string, char* bin_code, LABELS* labels, int* free_label, char const* start, int num_compilation);
#endif //CPU_ASSM_H