#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstring>

#define _DEBUD

#ifdef _DEBUD
#define ASSERT( x ) assert((x))
#elif ASSERT( x ) x
#endif

typedef double data_t;

const data_t Poison_for_new_data = -42;
const data_t Poison_for_delete_data = 42;

enum {
    Memory_error  = -1,
    Destroy_fault = 4,
    Bad_size = 5,
    Headache = 12121212,
    Tail_withered = 21212,
    Bad_links = 123

};

struct List_pointer_Elem{
    data_t data;
    List_pointer_Elem* previous;
    List_pointer_Elem* next;
};

struct my_pointer_List{
  List_pointer_Elem* head;
  List_pointer_Elem* tail;
  size_t counter;
  data_t hash;
  int error;
};

//----------------------------------------------------------------------------------------------------------------------
//functions for pointer's List
//----------------------------------------------------------------------------------------------------------------------

//______________________________________________________________________________________________________________________
//! This function create your List
//!
//! \return pointer of your NEW list
my_pointer_List* List_create();

//______________________________________________________________________________________________________________________
//! This function create new elment for List
//!
//! \return pointer of new element
List_pointer_Elem* create_elem();

//______________________________________________________________________________________________________________________
//! This function delete your list
//!
//! \param List your list
//! \return 1 if it's Ok and 0 if it has problems
bool List_destruct(my_pointer_List** List);

//______________________________________________________________________________________________________________________
//! This function remove element to your list
//!
//! \param List your list
//! \param front element after whom your want to create new element
//! \param new_data data that you want to push in your List
//! \return - pointer of the new element
//! \Note if you don't know head(only before creation) you must use this function with only two parameters
List_pointer_Elem* List_remoov(my_pointer_List* List, List_pointer_Elem* front, data_t new_data);

//______________________________________________________________________________________________________________________
//! This function delete element from List with known pointer of element
//!
//! \param List your list
//! \param elem pointer of your element
//! \return 1 if it's Ok and 0 if it has problems
bool List_delete(my_pointer_List* List, List_pointer_Elem* elem);

//______________________________________________________________________________________________________________________
//! This is verificator
//!
//! \param List your List
//! \return 1 if it's Ok and 0 if it has problems
bool List_Ok(my_pointer_List* List);

//______________________________________________________________________________________________________________________
//!  This function will find elem of list with your number, but it is toooooooooooooooooooooooooooooo slow
//!
//! \param password1 first password for using this function
//! \param password2 second password for using this function
//! \param List your list
//! \return
List_pointer_Elem* Errrror_use_only_once_never_use_it_again(char* password1, char* password2, my_pointer_List* List, int num);

//______________________________________________________________________________________________________________________
//! This function create new head of List
//!
//! \param List your list
//! \return pointer of new head
//! \Note it changes List
List_pointer_Elem* new_head_of_List(my_pointer_List* List, data_t new_data);

//______________________________________________________________________________________________________________________
//! This functions print your list
//!
//! \param List your list
//! \return 1 if it's Ok and 0 if it has problems
bool dump(my_pointer_List* List);

//______________________________________________________________________________________________________________________
#endif //LIST_LIST_H
