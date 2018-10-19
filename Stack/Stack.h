//
// Created by max_l on 10.10.2018.
//

#ifndef UNTITLED4_QSTACK_H
#define UNTITLED4_QSTACK_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cassert>
#include <time.h>

#define SAFETY

#ifdef SAFETY
#define CHECK_HASH( object ) check_hash( object )
#else
#define CHECK_HASH( object )
#endif

//#ifdef CRYING_FUNC
#define CRY_PRINTF printf
#else
#define CRY_PRINTF
#endif

#define _DEBUGGER

#ifdef _DEBUGGER
#define ASSERT_OK( check ) \
{\
    if( !(check) )\
    {\
        printf("%s is failed in %d\n", #check, __LINE__);\
        Dump( check );\
        abort();\
    }\
};
#else
#define ASSERT_OK
#endif


#define assert( check )                                                                                                \
{                                                                                                                      \
    if( !(check) )                                                                                                     \
    {                                                                                                                  \
        printf("O, noooooo: %s in %s in %s, in %d line\n", #check, __FILE__, __PRETTY_FUNCTION__,  __LINE__);          \
        abort();                                                                                                       \
    }                                                                                                                  \
}

#define DUMPname( stack ) printf("Dump \"%s\" ", #stack);

#endif //UNTITLED4_QSTACK_H



typedef double data_t;

enum errors {
        Bad_address = -42,
        Bad_size = -178,
        Bad_capasity = 2147483640,
        Memory_fault = 2147483645,
        Pop_zero_size = 2147483643,
        Killed_struct_canary = 2147483642,
        Killed_Stack_canary = 2147483641,
        Size_bigger_capasity = 2147483639,
        Hash_was_changed = 42
};

const int Error = 0;
const data_t Poison_for_data = NAN;
const size_t Poison_for_size = -1;

const int Shift = 9;

struct Stack
{
    size_t struct_canary_left;

    data_t* Stack;
    size_t  size;
    size_t  capasity;
    int     errors;
    data_t* left_canary;
    data_t* right_canary;
    data_t  hash;

    size_t struct_canary_right;
};

Stack* stackCreate(size_t len = 0);
int stack_destroy(Stack** mystack);
int stackOk(Stack* mystack);
int stackPush(Stack* mystack, data_t element);
data_t stackPop(Stack* mystack);
int stackClear(Stack* mystack);
int stackIncrease(Stack* mystack);
int stackReduce(Stack* mystack);
int Dump(Stack* mystack);
void Assert_OK(Stack* mystack);
data_t calc_hash(Stack* mystack);
bool check_hash(Stack* mystack);