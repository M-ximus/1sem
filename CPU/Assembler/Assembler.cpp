#include "Assm.h"
#include "Common.h"
//#include "C:\Program Files (x86)\TX\TXLib.h"

char* Assembler(int source_code, FILE* byte_code)
{
    mystring* Code = create_array_of_strings_fromFile(source_code);
    assert(Code != nullptr);
    char* buff = Code[0].String;
    unsigned int size = calc_no_zero_Addresses(Code);
    LABELS* labels = (LABELS*) calloc(Num_of_labels, sizeof(*labels));
    char* bin_code = compile(Code, labels);

    printf("%s: pos in bin code = %d\n", labels->word, labels->pos_in_bin_code);
    if ( fwrite(bin_code, 1, size * (3 + 8 * size_of_max_operation), byte_code) < 10)
    {
        printf("write errror");
        abort();
    }
    Print_bin_code(Code);
    destroyer(buff, Code);

    for (int i = 0; i < Num_of_labels; i++)
    {
        if (labels[i].word != nullptr)
            free(labels[i].word);
    }
    free(labels);

    return nullptr;
    //return bin_code;
}

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
            buff[i] = '\0';
            cut_comment(addressesStrings[pos_in_addressesStrings - 1].String);
            pos_in_addressesStrings++;
        }
    }
    cut_comment(addressesStrings[pos_in_addressesStrings - 1].String);
    addressesStrings[numStrings].String = nullptr;

    return addressesStrings;
}

//!
//! \param string
//! \return

int cut_comment(char* string)
{
    assert(string != nullptr);

    char* new_end = strchr(string, ';');
    if (new_end != nullptr)
        *new_end = '\0';
    else
        return -1;

    return 0;
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
//! This function print array in line
//!
//! \param Code - your array of strings
//----------------------------------------------------------------------------------------------------------------------
void Print_bin_code(mystring* Code)
{
    assert(Code != nullptr);

    size_t i = 0;
    while(Code[i].String != nullptr)
    {
        printf("%s\n", Code[i].String);
        i++;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//!
//----------------------------------------------------------------------------------------------------------------------

char* compile(mystring* Code, LABELS* labels)
{
    assert(Code != nullptr);

    size_t operations_number = calc_no_zero_Addresses(Code);
    char* bin_code = (char*) calloc(operations_number * (3 + 8 * size_of_max_operation), sizeof(char));
    assert(bin_code != nullptr);
    char const* start = bin_code;

    int pos_code = 0;
    int free_label = 0;
    for(int i = 0; Code[i].String != nullptr && i < operations_number; i++)// warning
        pos_code += write_comand(Code[i].String, bin_code + pos_code, labels, &free_label, start, 0);

    //printf("i am alive");

    pos_code = 0;
    for(int i = 0; Code[i].String != nullptr && i < operations_number; i++)// warning
        pos_code += write_comand(Code[i].String, bin_code + pos_code, labels, &free_label, start, 1);

    return bin_code;

}

//----------------------------------------------------------------------------------------------------------------------
//!
//----------------------------------------------------------------------------------------------------------------------

int write_comand(char* string, char* bin_code, LABELS* labels, int* free_label, char const* start, int num_compilation)
{
    assert(string != nullptr);
    assert(bin_code != nullptr);
    assert(start != nullptr);
    assert(labels != nullptr);

    int shift = 0;

    char buf[80] = "";
    sscanf(string, "%s", buf);
    if (buf[79] != '\0')
    {
        printf("toooooooo long string, sorry. But i can give you a cat) =^..^= ");
        abort();
    }

    int num_com = -1;
    #define DEF_CMD( name, num, func_asm, func_cpu) if (buf[0] == ':')\
    {\
        if (*free_label == Num_of_labels - 1 && num_compilation == 0)\
        {\
            printf("Errorororor too much labels, go to market and buy Johny Walker and I allow you to put more labels");\
            abort();\
        }\
        if (num_compilation == 0)\
        {\
            labels[*free_label].word = strdup(buf + 1);\
            labels[*free_label].pos_in_bin_code = bin_code - start;\
            (*free_label)++;\
        }\
        shift = 0;\
        return shift;\
    }\
    else if (strcmp(buf, #name) == 0)\
        num_com = num;

    #define DEF_MEM( name, num_arg)  ;
    #define DEF_REG( name, num_arg)  ;

    #include "comand.h"

    #undef DEF_CMD
    #undef DEF_REG
    #undef DEF_MEM

     if (num_com == -1)
     {
         printf("syntax error, i don't know this - %s", buf);
         abort();
     }

    switch(num_com)
    {
        #define DEF_CMD( name, num, func_asm, func_cpu) case num:\
        {\
            bin_code[0] = CMD_##name;\
            func_asm\
            break;\
        }
        #define DEF_MEM( name, num_arg)  ;
        #define DEF_REG( name, num_arg)  ;

        #include "comand.h"

        #undef DEF_CMD
        #undef DEF_REG
        #undef DEF_MEM
    }
    return shift;
}

//!
//! \param name_of_label
//! \param labels
//! \param num_of_last
//! \return
 int search_in_labels(char* name_of_label, LABELS* labels, int num_of_last)
 {
    assert(name_of_label != nullptr);
    assert(labels != nullptr);

    for(int i = 0; i < num_of_last; i++)
    {
        if (strcmp(labels[i].word, name_of_label) == 0)
            return labels[i].pos_in_bin_code;
    }

     return -1;
 }