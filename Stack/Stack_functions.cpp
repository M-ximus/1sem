#include "Stack.h"

//----------------------------------------------------------------------------------------------------------------------
//! This function create stack
//! \param len - capasity of stack
//! \return - address of new stack
//----------------------------------------------------------------------------------------------------------------------

Stack* stackCreate(size_t len)
{
    Stack* myStack = (Stack*) calloc(1, sizeof(*myStack));
    if (myStack == nullptr)
        return nullptr;

    srand(time(nullptr));
    myStack->struct_canary_left = myStack->struct_canary_right = rand();

    myStack->hash = 0;

    if(len == 0)
    {
        myStack->capasity = 0;
        myStack->size = 0;
    }
    else
    {
        myStack->capasity = len;
        myStack->size = 0;
        myStack->Stack = (data_t*) calloc(len, sizeof(*(myStack->Stack)));
        if (myStack->Stack == nullptr)
        {
            free(myStack);
            return nullptr;
        }
    }

    return myStack;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function check your stack and put number of error in the mystack->errors
//! \param mystack - address of your stack
//! \return - 1 if it Ok
//----------------------------------------------------------------------------------------------------------------------

bool stackOk(Stack* mystack)
{
    if (mystack == nullptr)
    {
        CRY_PRINTF("!!!Bad address of your struct!!!");
        return 0;
    }

    if (mystack->struct_canary_left != mystack->struct_canary_right)
    {
        mystack->errors = Killed_struct_canary;// mutable
        CRY_PRINTF("!!!Struct canary died!!!");
        return 0;
    }

    if (mystack->left_canary[1] != mystack->right_canary[1] && mystack->right_canary[0] == 0 && mystack->left_canary[0] == 0)
    {
        mystack->errors = Killed_Stack_canary;
        CRY_PRINTF("!!!Stack canary died!!!");
    }

    if (mystack->size == -1)
    {
        mystack->errors = Bad_size;
        CRY_PRINTF("!!!Bad size of your struct!!!");
        return 0;
    }

    if (mystack->capasity == -1)
    {
        mystack->errors = Bad_capasity;
        CRY_PRINTF("!!!Bad capasity of your struct!!!");
        return 0;
    }

    if (mystack->size > mystack->capasity)
    {
        mystack->errors = Size_bigger_capasity;
        CRY_PRINTF("!!!size bigger capasity!!!");
        return 0;
    }

    if(CHECK_HASH(mystack) + 0 != 0)
    {
        mystack->errors = Hash_was_changed;
        CRY_PRINTF("!!!Somebody(YOU!) broke your Stack. Hash was changed!!!");
        return 0;
    }
    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function push element in your stack
//! \param mystack - address of ypur stack
//! \param element - element, that you want to push
//! \return - 1 if it Ok and 0 if it have problems
//----------------------------------------------------------------------------------------------------------------------

int stackPush(Stack* mystack, data_t element)
{
    ASSERT_OK(mystack);/////////////////////////////////////////////////////////////////////////////////////////////////

    assert(element == element);

    if (mystack->size == mystack->capasity)
        stackIncrease(mystack);

    mystack->Stack[(mystack->size)] = element;
    mystack->size++;

    ASSERT_OK(mystack);

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function pop one element from the end of your stack
//! \param mystack - address of your stack
//! \return - element of your stack
//!
//! \Note - return the same stack and Memory_fault if we have problems with realloc
//----------------------------------------------------------------------------------------------------------------------

data_t stackPop(Stack* mystack)
{
    Assert_OK(mystack);

    if (mystack->size == 0)
    {
        mystack->errors = Pop_zero_size;
        return NAN;
    }

    data_t last_element = mystack->Stack[(mystack->size - 1)];
    mystack->Stack[(mystack->size - 1)] = Poison_for_data;
    (mystack->size)--;

    if ((mystack->size == (mystack->capasity / 4)) && (mystack->capasity > 4))
        stackReduce(mystack);

    Assert_OK(mystack);

    return last_element;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function clear your stack(push NAN)
//! \param mystack address of your stack
//! \return - 1 if all Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stackClear(Stack* mystack)
{
    Assert_OK(mystack);

    memset(mystack->Stack, NAN, mystack->capasity);
    free(mystack->Stack);

    mystack->Stack = nullptr;
    mystack->capasity = 0;
    mystack->size = 0;

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function destroy your stack
//! \param mystack - address of your stack
//! \return - 1 if it Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stack_destroy(Stack** mystack)
{
    assert(mystack);
    Assert_OK(*mystack);

    for(size_t i = ((*mystack)->size); i > 0; i--)
        (*mystack)->Stack[i] = NAN;
    free((*mystack)->Stack);
    (*mystack)->Stack = nullptr;

    (*mystack)->size = Poison_for_size;
    (*mystack)->capasity = Poison_for_size;

    free(*mystack);
    *mystack = nullptr;

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function increase your stack if memory end
//! \param mystack - address of your stack
//! \return - 1 if it Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stackIncrease(Stack* mystack)
{
    Assert_OK(mystack);

    data_t *copy = mystack->Stack;
    size_t len;
    if (mystack->capasity == 0)
        len = 1;
    else
        len = (mystack->capasity) * 2;

    mystack->Stack = (data_t *) realloc(mystack->Stack, len * sizeof(*(mystack->Stack)));
    if (mystack->Stack == nullptr)
    {
        mystack->Stack = copy;
        mystack->errors = Memory_fault;
        return Error;
    }
    mystack->capasity = len;

    Assert_OK(mystack);

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function reduce your stack
//! \param mystack - address of your stack
//! \return - 1 if it ok, else 0. Put num of error if it have problems
//----------------------------------------------------------------------------------------------------------------------

int stackReduce(Stack* mystack)
{
    Assert_OK(mystack);

    data_t *newStack = (data_t *) realloc(mystack->Stack, (mystack->capasity) / 2 * sizeof(*(mystack->Stack)));
    if (newStack == nullptr) {
        mystack->errors = Memory_fault;
        return Error;
    }
    mystack->Stack = newStack;
    mystack->capasity = (mystack->capasity) / 2;

    Assert_OK(mystack);

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function for debugging qStack. It print all information(and can help you to find problems)
//! \param mystack - address of your stack
//! \return - 1 if mystack doesn't have problems, else 0
//----------------------------------------------------------------------------------------------------------------------

int Dump(Stack* mystack)
{
    int check = 1;

    DUMPname(mystack);
    printf("[%p]", mystack);
    if (mystack == nullptr)
    {
        printf(" (ERROR!!!!!!!!!!!!!!!)");
        check = 0;
        return check;
    }
    printf("\n{\n");

    printf("\tcapasity = %ld\n", mystack->capasity);
    if (mystack->capasity == Poison_for_size)
    {
        printf(" ERROR!!!!!!!!!!!!!");
        check = 0;
    }
    printf("\n");

    printf("\tsize = %ld", mystack->size);
    if (mystack->capasity < mystack->size || mystack->size == Poison_for_size)
    {
        printf(" ERROR!!!!!!!!!!!!!");
        check = 0;
    }
    printf("\n");

    printf("\tStack %p\n", mystack->Stack);
    for(size_t i = 0; i < mystack->size && mystack->size != Poison_for_size && mystack->capasity != Poison_for_size; i++)
    {
        printf("\t*[%ld] = %lg", i, mystack->Stack[i]);
        if (mystack->Stack[i] == Poison_for_data)
            printf(" ERROR!!!!!!!!!!");
        printf("\n");
    }

    for(size_t i = mystack->size; i < mystack->capasity && mystack->size != Poison_for_size && mystack->capasity != Poison_for_size; i++)
    {
        printf("\t [%ld] = %lg", i, mystack->Stack[i]);
        if (mystack->Stack[i] == Poison_for_data)
            printf(" ERROR!!!!!!!!!!");
        printf("\n");
    }

    printf("}\n");

    return check;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function check mystack and if it bad print all debug information
//! \param mystack - address of your stack
//----------------------------------------------------------------------------------------------------------------------

void Assert_OK(Stack* mystack)
{
    if (!stackOk(mystack))
    {
        Dump(mystack);
        assert(mystack);
    }
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate hash sum by my special algorithm
//!
//! \param mystack - address of your stack
//! \return - hash
//----------------------------------------------------------------------------------------------------------------------

data_t calc_hash(Stack* mystack)
{
    long long hash = 0;

    for(int i = 0; i < mystack->size; i++)
    {
        int shift = (i + Shift) % 64;
        long long binary_num = (long long) mystack->Stack[i];

        long long circular_num = binary_num << shift;
        long long end_num      = binary_num >> shift;

        circular_num = circular_num | end_num;

        hash = hash ^ circular_num;
    }

    return (data_t) hash;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function compare new hash with hash of stack
//!
//! \param mystack - address of your stack
//! \return - false if mystack == nullptr or stack was broken else true
//----------------------------------------------------------------------------------------------------------------------
bool check_hash(Stack* mystack)
{
    if (mystack == nullptr)
        return false;

    if(calc_hash(mystack) == mystack->hash)
        return true;

    return false;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function recalculate hash when an element was poped
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------
data_t recalc_Pop_hash(Stack* mystack)
{

}

//----------------------------------------------------------------------------------------------------------------------
//! THis function recalculate hash when new element was pushed
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------
data_t recalc_Push_hash(Stack* mystack)
{

}