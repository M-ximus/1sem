#include "enc.h"

//----------------------------------------------------------------------------------------------------------------------
//! This function create array of strings from file
//!
//! @param[in] int inFile - address of the file
//!
//! @return address of the array of strings
//!
//! @Note In the end of array of strings we have address nullptr!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------------------------------------------------------

mystring* create_array_of_strings_fromFile(int inFile)
{
    assert(inFile > 0);

    char* buff = fillBuff(inFile);
    assert(buff != nullptr);

    mystring* addressesStrings = fillAddress(buff);

    assert(addressesStrings != nullptr);

    return addressesStrings;
}

//----------------------------------------------------------------------------------------------------------------------
//! Function that create buffer and fill them from reading file
//!
//! @param[in] int inFile - address of reading file
//!
//! @return - address of this buffer
//----------------------------------------------------------------------------------------------------------------------

char* fillBuff(int inFile)
{
    assert(inFile > 0);

    long int numSymbols = calcSize(way_to_in);
    assert(numSymbols != 0);

    char* buff = (char*) calloc(numSymbols, sizeof(*buff));
    assert(buff != nullptr);

    long int error = read(inFile, buff, numSymbols);
    assert(error == numSymbols);

    return buff;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of symbols in file
//!
//! @param[in] const char* inFile - full name of file
//!
//! @return - number of symbols in reading file
//!
//! @Note - this function return position of reading symbol to start of file
//----------------------------------------------------------------------------------------------------------------------

long int calcSize(const char* inFile)
{
    assert(inFile != nullptr);

    struct _stat fileInfo;
    _stat(inFile, &fileInfo);

    return fileInfo.st_size;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of special symbols in array
//!
//! @param[in] char* buff- address of array
//! @param[in] char symbol - symbol that we need find
//!
//! @return - number of strings
//!
//! @Note - this function return position of reading symbol to start of file
//----------------------------------------------------------------------------------------------------------------------

size_t calc_number_of_special_symbol(char* buff, char symbol)
{
    assert(buff != nullptr);

    size_t numSymbols = 1;
    for(char* pos = strchr(buff, symbol);pos;pos = strchr(pos + 1, symbol))
        numSymbols++;

    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function create array of the strings and fill it
//!
//! @param[in] char* buff - buffer with strings that we divide
//! @param[in] size_t numStrings - number of the strings in buffer
//!
//! @return - new struct with addresses of strings and their length
//!
//! @Note this array like matrix
//----------------------------------------------------------------------------------------------------------------------

mystring* fillAddress(char* buff)
{
    assert(buff != nullptr);

    size_t numStrings = calc_number_of_special_symbol(buff, '\n');
    assert(numStrings > 0);

    mystring* addressesStrings = (mystring*) calloc(numStrings + 1, sizeof(*addressesStrings));
    addressesStrings[0].String = buff;

    int pos_in_addressesStrings = 1;
    char* first_pos = buff;
    for(int i = 1; buff[i] != '\0' && pos_in_addressesStrings < numStrings; i++)// j - pos
    {
        if (buff[i] == '\r')
        {
            addressesStrings[pos_in_addressesStrings].String = buff + i + 2;
            addressesStrings[pos_in_addressesStrings - 1].len = buff + i + 1 - first_pos;
            first_pos = addressesStrings[pos_in_addressesStrings].String;
            pos_in_addressesStrings++;
            buff[i] = '\0';
        }
    }
    addressesStrings[numStrings].String = nullptr;

    return addressesStrings;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function sort array of strings
//!
//! @param[in] int numStrings - number of strings in array
//! @param[out] mystring* addressesStrings - input struct with addresses of strings(double-array)
//!
//! @return mystruct* addressesStrings - input array, but sorted
//----------------------------------------------------------------------------------------------------------------------

mystring* encyclopediaSorter_from_start(mystring* addressesStrings, size_t numStrings)
{
    assert(addressesStrings != nullptr);

    if (numStrings == 0)
        numStrings = calc_no_zero_Addresses(addressesStrings);
    assert(numStrings > 0);

    qsort(addressesStrings, numStrings, sizeof(*addressesStrings), compare_from_start);

    return addressesStrings;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function sort array of strings from last symbol(for rhyme)
//!
//! @param[in] int numStrings - number of strings in array
//! @param[out] mystring* addressesStrings - input struct with addresses of strings(double-array)
//!
//! @return mystruct* addressesStrings - input array, but sorted from last element
//----------------------------------------------------------------------------------------------------------------------

mystring* encyclopediaSorter_from_end(mystring* addressesStrings, size_t numStrings)
{
    assert(addressesStrings != nullptr);

    if (numStrings == 0)
        numStrings = calc_no_zero_Addresses(addressesStrings);
    assert(numStrings > 0);

    qsort(addressesStrings, numStrings, sizeof(*addressesStrings), compare_from_end);

    return addressesStrings;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function compare two strings from first element (compare only alphabets symbols)
//!
//! @param[in] firstStruct - address of mystring-type, first string for comparing
//! @param[in] secondStruct - address of mystring-type, second string for comparing
//!
//! @return positive int if first > second, negative if first < second and zero if first = second
//----------------------------------------------------------------------------------------------------------------------

int compare_from_start(const void* firstStruct, const void* secondStruct)
{
    char* firstString = ((mystring*)firstStruct)->String;
    char* secondString = ((mystring*)secondStruct)->String;
    int first_len = ((mystring*)firstStruct)->len;
    int second_len = ((mystring*)secondStruct)->len;

    int result = 0;
    int pos_first = 0, pos_second = 0;
    while(firstString[pos_first] != '\0' && secondString[pos_second] != '\0' && result == 0)
    {
        if(isalpha(firstString[pos_first]) == 0 && isalpha(secondString[pos_second]) == 0)
        {
            pos_first++;
            pos_second++;
        }
        else if(isalpha(firstString[pos_first]) == 0)
            pos_first++;
        else if(isalpha(secondString[pos_second]) == 0)
            pos_second++;
        else
        {
            result = firstString[pos_first] - secondString[pos_second];
            pos_first++;
            pos_second++;
        }
    }

    if(result)
        return result;
    else if(first_len > second_len)
        return 1;
    else if(first_len < second_len)
        return -1;
    else
        return 0;

}

//----------------------------------------------------------------------------------------------------------------------
//! This function prints poem to txt-file without zero-strings and rubber
//!
//! @param[in] char** addressesStrings - array of strings for printing
//! @param[in] int numStrings - number of strings
//! @param[out] FILE* outFile - address of the file for result
//!
//! @return zero if all is Ok
//----------------------------------------------------------------------------------------------------------------------

void printText(FILE* outFile, mystring* addressesStrings, size_t numStrings)
{
    assert(outFile != nullptr);
    assert(addressesStrings != nullptr);

    if (numStrings == 0)// kill
        numStrings = calc_no_zero_Addresses(addressesStrings);
    assert(numStrings > 0);

    for(int i = 0; i < numStrings; i++)
    {
        if(addressesStrings[i].String[0] == '\0')
            continue;
        fprintf(outFile, "%s\n", addressesStrings[i].String);
    }
}

//----------------------------------------------------------------------------------------------------------------------
//! This function the third part for working with arrays of strings. It destroys all memory that was allocated
//!
//! @param[in] char* buff - first param that need to destroy
//! @param[out] mystring* addressesStrings - second param that need to destroy
//!
//! @Note we destroys buffer earlier than addresses of strings, because it's a part of addresses of strings
//----------------------------------------------------------------------------------------------------------------------

void destroyer(char* buff, mystring* addressesStrings)
{
    free(buff);
    free(addressesStrings);
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of no-zero addresses
//!
//! @param[in] mystring* addressesStrings - array of strings
//!
//! @return number of no-zero addresses
//!
//! @Note WARNING!!!!!!!!!! addressesStrings must have nullptr in the end
//----------------------------------------------------------------------------------------------------------------------

size_t calc_no_zero_Addresses(mystring* addressesStrings)
{
    assert(addressesStrings != nullptr);

    size_t size = 1;
    for(; addressesStrings[size].String != nullptr; size++);
    return size;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function special for qsort for comparing strings from end(rhyme). Compare only alphabets symbols
//!
//! @param[in] firstStruct - address of mystring-type, first string for comparing
//! @param[in] secondStruct - address of mystring-type, second string for comparing
//!
//! @return positive int if first > second, negative if first < second and zero if first = second
//----------------------------------------------------------------------------------------------------------------------

int compare_from_end(const void* firstStruct, const void* secondStruct)
{
    char* firstString = ((mystring*)firstStruct)->String;
    char* secondString = ((mystring*)secondStruct)->String;
    size_t pos_first = (((mystring*)firstStruct)->len) - 1;
    size_t pos_second = (((mystring*)secondStruct)->len) - 1;
    size_t first_len = pos_first, second_len = pos_second;

    int result = 0;
    while(pos_first != 0 && pos_second != 0 && result == 0)
    {
        if(isalpha(firstString[pos_first]) == 0 && isalpha(secondString[pos_second]) == 0)
        {
            pos_first--;
            pos_second--;
        }
        else if(isalpha(firstString[pos_first]) == 0)
            pos_first--;
        else if(isalpha(secondString[pos_second]) == 0)
            pos_second--;
        else
        {
            result = firstString[pos_first] - secondString[pos_second];
            pos_first--;
            pos_second--;
        }
    }

    if(result)
        return result;
    else if(first_len > second_len)
        return 1;
    else if(first_len < second_len)
        return -1;
    else
        return 0;

}
