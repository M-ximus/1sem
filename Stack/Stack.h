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

#ifdef CRYING_FUNC
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
        Pop_zero_size = 2147483643
};

const int Error = 0;
const data_t Poison_for_data = NAN;
const size_t Poison_for_size = -1;

struct Stack
{
    data_t* Stack;
    size_t  size;
    size_t  capasity;
    int     errors;
};

struct safety_Stack
{
    data_t* Stack;
    data_t* all_data;
    size_t size;
    size_t capasity;
    size_t real_capasity;
    data_t left_canary[2];
    data_t right_canary[2];
    data_t checking_Sum;
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

