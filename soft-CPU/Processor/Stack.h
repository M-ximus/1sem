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

//#include <cassert>
#include <time.h>

//----------------------------------------------------------------------------------------------------------------------

#define SAFETY
#define CRYING_FUNC
#define _DEBUGGER

#ifdef SAFETY
#define CHECK_HASH( object ) check_hash( object )
#else
#define CHECK_HASH( object )
#endif

#ifdef CRYING_FUNC
#define CRY_PRINTF printf
#else
#define CRY_PRINTF
#endif

#ifdef _DEBUGGER
#define ASSERT_OK( check )                                                                                             \
{                                                                                                                      \
    if( !(stackOk((check))) )                                                                                             \
    {                                                                                                                  \
        printf("%s is failed in %d\n", #check, __LINE__);                                                              \
        Dump( check );                                                                                                 \
        abort();                                                                                                       \
    }\
};
#else
#define ASSERT_OK( check ) stackOk( check );
#endif

#define assert( check )                                                                                            \
{                                                                                                                      \
    if( !(check) )                                                                                                     \
    {                                                                                                                  \
        printf("O, noooooo: %s in %s in %s, in %d line\n", #check, __FILE__, __PRETTY_FUNCTION__,  __LINE__);          \
        abort();                                                                                                       \
    }                                                                                                                  \
}

#define DUMPname( stack ) printf("Dump \"%s\" ", #stack);

#endif //UNTITLED4_QSTACK_H

//----------------------------------------------------------------------------------------------------------------------

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
const data_t Stack_canary = 123456;

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

//----------------------------------------------------------------------------------------------------------------------
//! This function create stack
//! \param len - capasity of stack
//! \return - address of new stack
//----------------------------------------------------------------------------------------------------------------------
Stack* stackCreate(size_t len = 0);

//----------------------------------------------------------------------------------------------------------------------
//! This function destroy your stack
//! \param mystack - address of your stack
//! \return - 1 if it Ok, else 0
//----------------------------------------------------------------------------------------------------------------------
int stack_destroy(Stack** mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function check your stack and put number of error in the mystack->errors
//! \param mystack - address of your stack
//! \return - 1 if it Ok
//----------------------------------------------------------------------------------------------------------------------
bool stackOk(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function push element in your stack
//! \param mystack - address of your stack
//! \param element - element, that you want to push
//! \return - 1 if it Ok and 0 if it have problems
//----------------------------------------------------------------------------------------------------------------------
int stackPush(Stack* mystack, data_t element);

//----------------------------------------------------------------------------------------------------------------------
//! This function pop one element from the end of your stack
//! \param mystack - address of your stack
//! \return - element of your stack
//!
//! \Note - return the same stack and Memory_fault if we have problems with realloc
//----------------------------------------------------------------------------------------------------------------------
data_t stackPop(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function clear your stack(push NAN)
//! \param mystack address of your stack
//! \return - 1 if all Ok, else 0
//----------------------------------------------------------------------------------------------------------------------
int stackClear(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function increase your stack if memory end
//! \param mystack - address of your stack
//! \return - 1 if it Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stackIncrease(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function reduce your stack
//! \param mystack - address of your stack
//! \return - 1 if it ok, else 0. Put num of error if it have problems
//----------------------------------------------------------------------------------------------------------------------
int stackReduce(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function for debugging qStack. It print all information(and can help you to find problems)
//! \param mystack - address of your stack
//! \return - 1 if mystack doesn't have problems, else 0
//----------------------------------------------------------------------------------------------------------------------
int Dump(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function check mystack and if it bad print all debug information
//! \param mystack - address of your stack
//----------------------------------------------------------------------------------------------------------------------
void Assert_OK(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate hash sum by my special algorithm
//!
//! \param mystack - address of your stack
//! \return - hash
//----------------------------------------------------------------------------------------------------------------------
data_t calc_hash(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function compare new hash with hash of stack
//!
//! \param mystack - address of your stack
//! \return - false if mystack == nullptr or stack was broken else true
//----------------------------------------------------------------------------------------------------------------------
bool check_hash(Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function recalculate hash when an element was poped
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------
data_t recalc_Push_hash(const Stack* mystack);

//----------------------------------------------------------------------------------------------------------------------
//! This function recalculate hash when new element was pushed
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------
data_t recalc_Pop_hash(const Stack* mystack, data_t element);