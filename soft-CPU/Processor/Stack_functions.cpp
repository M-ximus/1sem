#include "C:\Program Files (x86)\TX\TXLib.h"
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
        data_t* copy = (data_t*) calloc(len + 4, sizeof(*(myStack->Stack)));
        if (copy == nullptr)
        {
            free(myStack);
            return nullptr;
        }

        myStack->left_canary =  copy;
        myStack->Stack =        copy + 2;
        myStack->right_canary = copy + 2 + len;

        myStack->left_canary[1] =  Stack_canary;
        myStack->right_canary[0] = Stack_canary;
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
        return false;
    }

    if (mystack->struct_canary_left != mystack->struct_canary_right)
    {
        mystack->errors = Killed_struct_canary;// mutable
        CRY_PRINTF("!!!Struct canary died!!!");
        return false;
    }

    if (mystack->left_canary != nullptr && mystack->right_canary != nullptr)
    {
        if(mystack->left_canary[1] != mystack->right_canary[1] &&
           mystack->right_canary[0] == 0 &&
           mystack->left_canary[0] == 0)
        {
            mystack->errors = Killed_Stack_canary;
            CRY_PRINTF("!!!Stack canary died!!!");
            return false;
        }
    }

    if (mystack->size == -1)
    {
        mystack->errors = Bad_size;
        CRY_PRINTF("!!!Bad size of your struct!!!");
        return false;
    }

    if (mystack->capasity == -1)
    {
        mystack->errors = Bad_capasity;
        CRY_PRINTF("!!!Bad capasity of your struct!!!");
        return false;
    }

    if (mystack->size > mystack->capasity)
    {
        mystack->errors = Size_bigger_capasity;
        CRY_PRINTF("!!!size bigger capasity!!!");
        return false;
    }

    if(CHECK_HASH(mystack) + 0 != 0)
    {
        mystack->errors = Hash_was_changed;
        CRY_PRINTF("!!!Somebody(YOU!) broke your Stack. Hash was changed!!!\n");
        return false;
    }
    return true;
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
    mystack->hash = recalc_Push_hash(mystack);

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
    ASSERT_OK(mystack);

    if (mystack->size == 0)
    {
        mystack->errors = Pop_zero_size;
        return NAN;
    }

    data_t last_element = mystack->Stack[(mystack->size - 1)];
    mystack->Stack[(mystack->size - 1)] = Poison_for_data;
    (mystack->size)--;

    if ((mystack->size <= (mystack->capasity / 4)) && (mystack->capasity > 4))
        stackReduce(mystack);

    mystack->hash = recalc_Pop_hash(mystack, last_element);

    ASSERT_OK(mystack);

    return last_element;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function clear your stack(push NAN)
//! \param mystack address of your stack
//! \return - 1 if all Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stackClear(Stack* mystack)
{
    ASSERT_OK(mystack);

    memset(mystack->Stack, 0, mystack->capasity);
    free(mystack->Stack);

    mystack->Stack = nullptr;
    mystack->capasity = 0;
    mystack->size = 0;
    mystack->hash = 0;

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function destroy your stack
//! \param mystack - address of your stack
//! \return - 1 if it Ok, else 0
//----------------------------------------------------------------------------------------------------------------------

int stack_destroy(Stack** mystack)
{
    ASSERT_OK(*mystack);


    for(size_t i = 0; i < (*mystack)->size; i++)
        (*mystack)->Stack[i] = NAN;
    free((*mystack)->left_canary);
    (*mystack)->Stack = nullptr;
    (*mystack)->left_canary = nullptr;
    (*mystack)->right_canary = nullptr;

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
    ASSERT_OK(mystack);

    size_t len = 0;
    if (mystack->capasity == 0)
        len = 1;
    else
        len = (mystack->capasity) * 2;

    data_t* copy = (data_t *) realloc(mystack->left_canary, (len + 4)* sizeof(*(mystack->Stack)));
    if (copy == nullptr)
    {
        mystack->errors = Memory_fault;
        return Error;
    }

    mystack->Stack =        copy + 2;
    mystack->left_canary =  copy;
    mystack->right_canary = copy + 2 + len;
    mystack->right_canary[0] = Stack_canary;
    mystack->right_canary[1] = 0;

    mystack->capasity = len;

    if (len == 1)
    {
        mystack->left_canary[0] =  0;
        mystack->left_canary[1] =  Stack_canary;
        mystack->right_canary[0] = Stack_canary;
        mystack->right_canary[1] = 0;
    }

    //ASSERT_OK(mystack);

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function reduce your stack
//! \param mystack - address of your stack
//! \return - 1 if it ok, else 0. Put num of error if it have problems
//----------------------------------------------------------------------------------------------------------------------

int stackReduce(Stack* mystack)
{
   // ASSERT_OK(mystack);

    size_t len = mystack->capasity / 2;
    data_t old_canary = *(mystack->right_canary);

    data_t* newStack = (data_t *) realloc(mystack->left_canary, (len + 4) * sizeof(*(mystack->Stack)));
    if (newStack == nullptr) {
        mystack->errors = Memory_fault;
        return Error;
    }

    mystack->left_canary = newStack;
    mystack->Stack = newStack + 2;
    mystack->right_canary = newStack + 2 + len;
    *(mystack->right_canary) = old_canary;
    mystack->capasity = len;

    //ASSERT_OK(mystack);

    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function for debugging qStack. It print all information(and can help you to find problems)
//! \param mystack - address of your stack
//! \return - 1 if mystack doesn't have problems, else 0
//----------------------------------------------------------------------------------------------------------------------

int Dump(Stack* mystack) {
    int check = 1;

    DUMPname(mystack);
    printf("[%p]", mystack);
    if (mystack == nullptr) {
        printf(" (ERROR!!!!!!!!!!!!)");
        check = 0;
        return check;
    }
    printf("\n{\n");

    printf("\tleft canary of structure = %u", mystack->struct_canary_left);
    if (mystack->struct_canary_left != mystack->struct_canary_right)
        printf(" (ERROR!!!!!!!!!!!!)");
    printf("\n\n");

    printf("\tcapasity = %u\n", mystack->capasity);
    if (mystack->capasity == Poison_for_size) {
        printf(" (ERROR!!!!!!!!!!!!)");
        check = 0;
    }
    printf("\n");

    printf("\tsize = %u", mystack->size);
    if (mystack->capasity < mystack->size || mystack->size == Poison_for_size) {
        printf(" (ERROR!!!!!!!!!!!!)");
        check = 0;
    }
    printf("\n\n");

    if (mystack->left_canary != nullptr)
    {
        printf("\tleft canaries of Stack: [0] = %g", mystack->left_canary[0]);
        if (mystack->left_canary[0] != 0)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf(", [1] = %g", mystack->left_canary[1]);
        if (mystack->left_canary[1] != Stack_canary)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf("\n");
    }

    printf("\tStack [%p]\n", mystack->Stack);
    for(size_t i = 0; i < mystack->size && mystack->size != Poison_for_size && mystack->capasity != Poison_for_size; i++)
    {
        printf("\t*[%u] = %lg", i, mystack->Stack[i]);
        if (mystack->Stack[i] == Poison_for_data)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf("\n");
    }

    for(size_t i = mystack->size; i < mystack->capasity && mystack->size != Poison_for_size && mystack->capasity != Poison_for_size; i++)
    {
        printf("\t [%u] = %lg", i, mystack->Stack[i]);
        if (mystack->Stack[i] == Poison_for_data)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf("\n");
    }

    if(mystack->right_canary != nullptr)
    {
        printf("\tright canaries of Stack: [0] = %g", mystack->right_canary[0]);
        if (mystack->right_canary[0] != Stack_canary)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf(", [1] = %g", mystack->right_canary[1]);
        if (mystack->right_canary[1] != 0)
            printf(" (ERROR!!!!!!!!!!!!)");
        printf("\n");
    }

    printf("\n\tright canary of structure = %u", mystack->struct_canary_left);
    if (mystack->struct_canary_left != mystack->struct_canary_right)
        printf(" (ERROR!!!!!!!!!!!!)");
    printf("\n");

    printf("\tYour hash %g\n", mystack->hash);

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
    if (mystack == nullptr)
        return NAN;

    long long hash = 0;

    for(int i = 0; i < mystack->size; i++)
    {
        int shift = (i + Shift) % 64;
        long long binary_num = (long long) mystack->Stack[i];

        long long circular_num = binary_num << shift;
        long long end_num      = binary_num >> (64 - shift);////////////////////////////////////////////////////////////

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
        return true;

    if(mystack->hash == calc_hash(mystack))
        return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function recalculate hash when an element was poped
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------

data_t recalc_Pop_hash(const Stack* mystack, data_t element)
{
    if (mystack == nullptr)
        return NAN;

    int position = (mystack->size + Shift) % 64;
    long long binary_num = (long long) element;

    long long circular_num = binary_num << position;
    long long end_num      = binary_num >> (64 - position);

    circular_num = circular_num | end_num;
    return (data_t) (circular_num ^ ((long long) mystack->hash));

}

//----------------------------------------------------------------------------------------------------------------------
//! This function recalculate hash when new element was pushed
//!
//! \param mystack - address of your stack
//! \return - new hash
//----------------------------------------------------------------------------------------------------------------------

data_t recalc_Push_hash(const Stack* mystack)
{
    if (mystack == nullptr)
        return NAN;

    if (mystack->size == 0)
        return 0;

    int position = (mystack->size - 1 + Shift) % 64;
    long long binary_element = (long long) mystack->Stack[mystack->size - 1];

    long long circular_num = binary_element << position;
    long long end_of_num   = binary_element >> (64 - position);

    circular_num = circular_num | end_of_num;

    return (data_t) (circular_num ^ ((long long) mystack->hash));
}