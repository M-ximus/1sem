//
// Created by max_l on 01.11.2018.
//
#include "Common.h"
#include "comand.h"
#include "soft_CPU.h"

int CPU(char* bynary_code, int machine_code)
{
    assert((bynary_code != nullptr) || (machine_code != -1));

    if (bynary_code == nullptr)
        bynary_code = fillBuff(machine_code);

    run(bynary_code);

    return -1;
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

    long int numSymbols = calcSize(way_to_run);
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

int run(char* bynary_code)
{
    assert(bynary_code != nullptr);

    int pc = 0;
    Stack* CPU_stack = stackCreate(start_size_for_CPU_stack);
    Stack* CALL_stack = stackCreate(start_size_for_CALL_stack);
    char* RAM = (char*) calloc(size_of_RAM, sizeof(*RAM));
    double* REGisters = (char*) calloc(num_of_reg, sizeof(*REGisters));

    assert(RAM != nullptr);
    assert(REGisters != nullptr);
    assert(CALL_stack != nullptr);
    assert(CPU_stack != nullptr);

    char command = -1;
    do
    {
        command = do_command(bynary_code, &pc, CPU_stack, CALL_stack, RAM, REGisters);
    } while(command != CMD_END && command != -1);

    stack_destroy(&CPU_stack);
    stack_destroy(&CALL_stack);
    RAM_destroy(&RAM);

    return 1;
}

//!

char do_command(char* bynary_code, int* pc, Stack* CPU_stack, Stack* CALL_stack, char* RAM, double* REGisters)
{
    switch(bynary_code[*pc])
    {
        #define DEF_CMD(name, num_com, func_asm, func_CPU) case CMD_##name:\
        {\
            (*pc)++;\
            func_CPU;\
            break;\
        }\

        #define DEF_MEM(name, num_arg)  ;
        #define DEF_REG(name, num_arg)  ;

        #include "comand.h"

        #undef DEF_CMD
        #undef DEF_MEM
        #undef DEF_REG

        default:
        {
            printf("Syntax error, i don't know %c command", bynary_code[*pc]);
            abort();
        }
    }
}

//!

int RAM_destroy(char** RAM)
{
    assert(RAM != nullptr);
    assert(*RAM != nullptr);

    for(int i = 0; i < size_of_RAM; i++)
        (*RAM)[i] = '\0';

    free(*RAM);
    *RAM = nullptr;

    return 1;
}