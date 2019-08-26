#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
//#include <Windows.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <cmath>

#define _DEBUG
#define FASTEST_PROGA

#ifdef _DEBUG
    #define ASSERT( x ) assert((x))
#else
    #ifdef FASTEST_PROGA
        #define ASSERT( x )
    #else
        #define ASSERT( x ) x
    #endif
#endif


typedef char data_t;

//char Poison_for_strings[] = "I lost your string";
//char Poison_for_new_strings[] = "Problems with code? Let's go to drink tea and speak with duc";
const int Poison_for_counter = 5051;

const char way_to_in[]  = "C:\\Users\\max_l\\CLionProjects\\Akinator\\questions.txt";
const char way_to_out[] = "C:\\Users\\max_l\\CLionProjects\\Akinator\\questions.txt";

enum{
    Memory_error  = -1,
    Link_error    = 24,
    Root_error    = 42,
    Counter_error = 256,
    Delete_error  = -5,
    Syntax_error  = 34
};

struct mystring
{
    char* String;
    size_t len;
};

struct vertex{
    data_t* data;
    vertex* parent;
    vertex* left_son;
    vertex* right_daughter;
};

struct Tree{
    vertex* root;
    size_t  counter;
    int     error;
};

struct counters
{
    int integer;
    size_t size;
};

//______________________________________________________________________________________________________________________

//! This function create tree and diploma of the Timiryazev Academy
//!
//! \return pointer of your new brilliant tree
//! \Note can't create Groot
Tree* Tree_construct();

//! This function delete your tree
//!
//! \return 1 if it's Ok and 0 if something has broken
bool Tree_distruct(Tree** myTree);

//! This function create new left vertex after your vertex
//!
//! \param myTree pointer of your tree
//! \param new_parent vertex after whom you want to add left new vertex
//! \return new left vertex
vertex* create_left_son(Tree* myTree, vertex* new_parent, data_t* info);

//! This function create new right vertex after your vertex
//!
//! \param myTree pointer of your tree
//! \param new_parent
//! \return new_parent vertex after whom you want to add right new vertex
vertex* create_right_daughter(Tree* myTree, vertex* new_parent, const data_t* info);

//! This function change your root to new and make new left son of new root
//!
//! \param myTree pointer of your tree
//! \return pointer of the new root
vertex* change_root_left(Tree* myTree);

//! This function change your root to new and make new right daughter of new root
//!
//! \param myTree pointer of your tree
//! \return pointer of the new root
vertex* change_root_right(Tree* myTree);

//! This function delete all branch that started from this vertex
//!
//! \param myTree your tree
//! \param Judah branch that you want to kill
//! \return true if it's Ok and false if something has broken
//! \Note you can use it to clear all tree
bool delete_branch(Tree* myTree, vertex* Judah);

//! This function create root of your tree
//!
//! \param myTree pointer of your tree
//! \return pointer of new root
//! \Note use it only for roots
vertex* root_create(Tree* myTree);

//! This function check your tree
//!
//! \param myTree pointer of your tree
//! \return true if it's ok and false if something has broken
bool Tree_ok(Tree* myTree);

//! This function save your tree to the file
//!
//! \param myTree pointer of your branch (vertex from whom we will start)
//! \param outFile opened file where will be save information
//! \return true if it's ok and false if something broken
bool save_tree(vertex* node, FILE* outFile);

//! This function create empty element of tree
//!
//! \param myTree pointer of your tree
//! \return pointer of the new element or nullptr if calloc can't working
vertex* create_empty_elem(Tree* myTree);

//! This function create new vertex if we want to control all things in tree
//!
//! \param myTree pointer of your tree
//! \param left_vertex pointer to the left son
//! \param right_vertex pointer to the right daughter
//! \param info information that you put into vertex
//! \return pointer of new vertex
//! \Note this vertex don't have parent!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
vertex* create_vertex(Tree* myTree, vertex* left_vertex, vertex* right_vertex, data_t* info = nullptr);

//! This function create tree from special commands from file
//!
//! \param inFile pointer of your file in special format
//! \return pointer of your tree
Tree* create_tree_from_file(mystring* strings);

//! This function return pointer of copy of your data from dinamic memory
//!
//! \param info data that you need to copy
//! \return pointer of copy
data_t* put_info(data_t* info);

//! This function check all branches of this vertex
//!
//! \param node pointer of your vertex
//! \return true if all branches ok and false if it have some problems
bool Linkers_ok(vertex* node);

//! This function check only one vertex for linking with parents
//!
//! \param node pointer of your vertex
//! \return true if all was ok and false if something has broken
bool check_links(vertex* node);

//! This function create new branch(DON'T USE FOR GITHUB)
//!
//! \param myTree pointer of your tree
//! \param strings pointer of array of your strings
//! \param num_string counters
//! \return pointer to new node
vertex* create_branch(Tree* myTree, mystring* strings, counters* num_string);

//----------------------------------------------------------------------------------------------------------------------
//! This function create array of strings from file
//!
//! @param[in] int inFile - address of the file
//!
//! @return address of the array of strings
//!
//! @Note In the end of array of strings we have address nullptr!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------------------------------------------------------

mystring* create_array_of_strings_fromFile(int inFile);

//----------------------------------------------------------------------------------------------------------------------
//! Function that create buffer and fill them from reading file
//!
//! @param[in] int inFile - address of reading file
//!
//! @return - address of this buffer
//----------------------------------------------------------------------------------------------------------------------

char* fillBuff(int inFile);

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of symbols in file
//!
//! @param[in] const char* inFile - full name of file
//!
//! @return - number of symbols in reading file
//!
//! @Note - this function return position of reading symbol to start of file
//----------------------------------------------------------------------------------------------------------------------

long int calcSize(const char* inFile);

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

size_t calc_number_of_special_symbol(char* buff, char symbol);

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

mystring* fillAddress(char* buff);

//----------------------------------------------------------------------------------------------------------------------
//! This function the third part for working with arrays of strings. It destroys all memory that was allocated
//!
//! @param[in] char* buff - first param that need to destroy
//! @param[out] mystring* addressesStrings - second param that need to destroy
//!
//! @Note we destroys buffer earlier than addresses of strings, because it's a part of addresses of strings
//----------------------------------------------------------------------------------------------------------------------

void destroyer(char* buff, mystring* addressesStrings);

//! This function print tree from file
//!
//! \param Code array of your strings
void Print_tree(mystring* Code);

//! This function ask you and found leave with word
//!
//! \param node pointer of current node(searching in this branch)
//! \return pointer of your leave with word
vertex* asking(vertex* node);

//! This function push new elem into tree with question and new word
//!
//! \param node pointer of node with word whose akinator didn't guess
//! \param myTree pointer of your tree
//! \return pointer to node with new question
vertex* push_new(vertex* node, Tree* myTree);

//______________________________________________________________________________________________________________________

#endif //AKINATOR_TREE_H

