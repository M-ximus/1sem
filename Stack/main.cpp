//! @file main.cpp
//----------------------------------------------------------------------------------------------------------------------
//!
//!
//----------------------------------------------------------------------------------------------------------------------
//! @mainpage
//! - main.cpp
//----------------------------------------------------------------------------------------------------------------------

#include "Stack.h"
#include "DUMP.h"

int main() {
    Stack* testStack = stackCreate();
    assert(stackOk(testStack));

    //safity_Stack* safeStack = safaty_stackCreate();

    stackPush(testStack, 42);
    assert(42 == stackPop(testStack));

    size_t num_Pop = 7;
    for(int i = 0; i < num_Pop; i++)
        stackPush(testStack, 1);
    assert(num_Pop == testStack->size);

    //printf("%g", calc_hash(testStack));

    for(int i = num_Pop; i > 0; i--)
        assert(1 == stackPop(testStack));
    assert(testStack->size == 0);

    stackClear(testStack);
    assert(testStack->size == 0);

    stack_destroy(&testStack);
    //assert(testStack->size == Poison_for_size);
    //assert(testStack->capasity == Poison_for_size);
    //assert(testStack->Stack == nullptr);

    //stackPush(testStack, 1);

    Stack* test_incilised_Stack = stackCreate(3);
    assert(stackOk(test_incilised_Stack));

    assert(test_incilised_Stack->capasity == 3);

    test_incilised_Stack->capasity = Poison_for_size;

    for(int i = 0; i < num_Pop; i++)
        stackPush(test_incilised_Stack, 1);
    assert(test_incilised_Stack->size == num_Pop);
    assert(test_incilised_Stack->capasity == 12);

    for(int i = num_Pop; i > 2; i--)
        assert(stackPop(test_incilised_Stack) == 1);
    assert(test_incilised_Stack->capasity == 6);

    stack_destroy(&test_incilised_Stack);

    return 0;
}
