#include "List.h"

my_pointer_List* List_create()
{
    my_pointer_List* List = (my_pointer_List*) calloc(1, sizeof(*List));
    if (List == nullptr)
        return List;

    List->head = nullptr;
    List->tail = nullptr;
    List->error   = 0;
    List->counter = 0;
    List->hash    = 0;

    return List;
}

//----------------------------------------------------------------------------------------------------------------------

List_pointer_Elem* create_elem()
{
    List_pointer_Elem* new_elem = (List_pointer_Elem*) calloc(1, sizeof(*new_elem));
    if (new_elem == nullptr)
        return new_elem;

    new_elem->next     = nullptr;
    new_elem->previous = nullptr;
    new_elem->data     = Poison_for_new_data;

    return new_elem;
}

//----------------------------------------------------------------------------------------------------------------------

bool List_destruct(my_pointer_List** List)
{
    ASSERT(List_Ok(*List));

    int num_elem = 0;
    int counter = (*List)->counter;
    for(; num_elem < counter; num_elem++)
    {
        if ((*List)->head == nullptr)
            break;
        List_delete(*List, (*List)->head);
    }

    if (num_elem != counter)
    {
        (*List)->error = Destroy_fault;
        return false;
    }

    free(*List);

    *List = nullptr;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

List_pointer_Elem* List_remoov(my_pointer_List* List, List_pointer_Elem* front, data_t new_data)
{
    ASSERT(List_Ok(List));
    assert(front != nullptr);
    assert(new_data == new_data);

    List_pointer_Elem* new_elem = create_elem();
    if (new_elem == nullptr)
    {
        List->error = Memory_error;
        return nullptr;
    }

    if (front != List->tail)
    {
        front->next->previous = new_elem;
        new_elem->next        = front->next;
        front->next           = new_elem;
        new_elem->previous    = front;
        new_elem->data        = new_data;
    }
    else
    {
        new_elem->next     = nullptr;
        List->tail         = new_elem;
        new_elem->previous = front;
        front->next        = new_elem;
        new_elem->data     = new_data;
    }

    (List->counter)++;

    ASSERT(List_Ok(List));

    return new_elem;
}

//----------------------------------------------------------------------------------------------------------------------

bool List_delete(my_pointer_List* List, List_pointer_Elem* elem)
{
    ASSERT(List_Ok(List));
    assert(elem != nullptr);

    if (elem != List->tail && elem != List->head)
    {
        elem->next->previous = elem->previous;
        elem->previous->next = elem->next;
    }
    else if (elem == List->head && elem != List->tail)
    {
        elem->next->previous = nullptr;
        List->head           = elem->next;
        elem->next           = nullptr;
    }
    else if (elem == List->tail && elem != List->head)
    {
        elem->previous->next = nullptr;
        List->tail           = elem->previous;
        elem->previous       = nullptr;
    }
    else
    {
        List->tail = nullptr;
        List->head = nullptr;
    }


    elem->data = Poison_for_delete_data;
    (List->counter)--;

    free(elem);

    ASSERT(List_Ok(List));

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool List_Ok(my_pointer_List* List)
{
    if (List == nullptr)
        return false;

    if(List->head == nullptr && List->tail == nullptr && List->counter == 0)
        return true;

    if (List->head == List->tail && List->counter != 1)
    {
        List->error = Bad_size;
        return false;
    }

    if (List->head->previous != nullptr)
    {
        List->error = Headache;
        return false;
    }

    if (List->tail->next != nullptr)
    {
        List->error = Tail_withered;
        return false;
    }

    int i = 1;
    List_pointer_Elem* tester = List->head;
    for(; i < List->counter; i++)
    {
        if (tester->next == nullptr || tester == List->tail)
            break;
        tester = tester->next;
    }

    if (List->counter != i || tester != List->tail)
    {
        List->error = Bad_links;
        return false;
    }

    return true;
}

List_pointer_Elem* new_head_of_List(my_pointer_List* List, data_t new_data)
{
    ASSERT(List_Ok(List));
    assert(new_data == new_data);

    List_pointer_Elem* new_head = create_elem();
    assert(new_head != nullptr);

    if (List->counter == 0)
    {
        List->tail         = new_head;
        List->head         = new_head;
        new_head->next     = nullptr;
        new_head->previous = nullptr;
        new_head->data     = new_data;
    }
    else
    {
        List->head->previous = new_head;
        new_head->next       = (List->head);
        new_head->previous   = nullptr;
        new_head->data       = new_data;
        List->head           = new_head;
    }

    (List->counter)++;

    ASSERT(List_Ok(List));

    return new_head;
}

//----------------------------------------------------------------------------------------------------------------------

List_pointer_Elem* Errrror_use_only_once_never_use_it_again(char* password1, char* password2, my_pointer_List* List, int num)
{
    ASSERT(List_Ok(List));
    assert(num >= 0);

    if(num > List->counter)
        return nullptr;

    const char true1[] = "12qw_Hgfo";
    const char true2[] = "vhddKG(@_sd";

    if(strcmp(password1, true1) || strcmp(password2, true2))
        return nullptr;

    List_pointer_Elem* elem = List->head;
    for(int i = 1; i < num; i++)
        elem = elem->next;

    ASSERT(List_Ok(List));

    return elem;
}

bool dump(my_pointer_List* List)
{
    if(List == nullptr)
    {
        printf("\nIt isn't a list\n");
        return false;
    }

    printf("List address [%p]\n\n", List);

    if(List->counter == 0)
    {
        printf("\t head[%p] tail[%p]", List->head, List->tail);
        return true;
    }

    printf(" counter = %d\n{\n", List->counter);

    int i = 1;
    List_pointer_Elem* cur_elem = List->head;
    for(; i < List->counter; i++)
    {
        printf("\tprev[%p] next[%p] data[%lg]", cur_elem->previous, cur_elem->next, cur_elem->data);
        if(cur_elem != List->tail && cur_elem->next->previous != cur_elem)
            printf("Error link");
        printf("\n");
    }
    printf("num elem = %d\n}\n", i);
    printf("Errors - %d", List->error);
    return true;
}