//
// Created by max_l on 18.12.2018.
//

#ifndef COMPILATOR_COMPILATOR_H
#define COMPILATOR_COMPILATOR_H

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include "C:\Program Files (x86)\TX\TXLib.h"

#ifdef _DEBUG
#define ASSERT( x ) assert((x))
#else
#ifdef FASTEST_PROGA
#define ASSERT( x )
#else
#define ASSERT( x ) x
#endif
#endif


#define sassert( x )                                                                                                    \
{                                                                                                                       \
    if (!(x))                                                                                                           \
    {                                                                                                                   \
        printf("Syntax error in %d line in %s", __LINE__, __PRETTY_FUNCTION__);                                         \
        if (pos != nullptr)                                                                                        \
            printf(" symbol is %c", *pos);                                                                         \
        abort();                                                                                                        \
    }                                                                                                                   \
}

#define CHECK_LEN( x ) \
{                                                                                                                       \
    if ((x) == pos)                                                                                                \
    {                                                                                                                   \
        printf("Errorororo I began cleaning your laptop\n");                                                            \
        printf("P.s. Don't panic, it was joke(maybe)))). Length = 0 in %s in %d line\n", __PRETTY_FUNCTION__, __LINE__);\
        if (pos != nullptr)                                                                                        \
            printf(" symbol is %c", *pos);                                                                         \
        abort();                                                                                                        \
    }                                                                                                                   \
}

typedef char data_t;

enum{
    Memory_error  = -1,
    Link_error    = 24,
    Root_error    = 42,
    Counter_error = 256,
    Delete_error  = -5,
    Syntax_error  = 34
};

enum type_of_data
{
    Operator = 30,
    Variable = 40,
    Number = 50,
    Function = 60,
    Standart_func = 70,
    Branching = 80,
    Inicialise = 90,
    Equality = 100,
    Body = 110,
    Condition = 120,
    Return = 130,
    Parameter = 140
};

struct var
{
    char* name_of_var;
    int position;
};

struct func
{
    char* name_of_func;
    int num_of_param;
    char* code;
};

struct mystring
{
    char*  String;
    size_t len;
};

struct data_for_rec_tree
{
    type_of_data type;
    char*        oper_name;
    char*        func_name;
    char*        stand_func_name;
    char*        branch_name;
    int          number;
    char*        var_name;
};

struct vertex
{
    data_for_rec_tree* data;
    vertex* parent;
    vertex* left_son;
    vertex* right_daughter;
};

struct Tree
{
    vertex* root;
    size_t  counter;
    int     error;
};

struct counters
{
    int integer;
    size_t size;
};


//----------------------------------------------------------------------------------------------------------------------
const char way_to_in[] = "C:\\Users\\max_l\\CLionProjects\\compilator\\Code.txt";
const char way_to_out[] = "C:\\Users\\max_l\\CLionProjects\\compilator\\Asm.txt";
const int num_of_var = 10;
const int num_of_func = 10;
//----------------------------------------------------------------------------------------------------------------------

mystring* create_array_of_strings_fromFile(int inFile);

char* fillBuff(int inFile);

long int calcSize(const char* inFile);

size_t calc_number_of_special_symbol(char* buff, char symbol);

mystring* fillAddress(char* buff);

int cut_comment(char* string);

size_t calc_no_zero_Addresses(mystring* addressesStrings);

void destroyer(char* buff, mystring* addressesStrings);

void print_mode(vertex* pos, FILE* dot_out);

void create_tree(vertex* pos, FILE* dot_out);

void tree_dot(Tree* nameTree, char* dot_out);

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
vertex* create_right_daughter(Tree* myTree, vertex* new_parent, data_t* info);

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
vertex* create_vertex(Tree* myTree, vertex* left_vertex, vertex* right_vertex);

//! This function return pointer of copy of your data from dinamic memory
//!
//! \param info data that you need to copy
//! \return pointer of copy
data_t* put_info(char* info);

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

int delete_data(data_for_rec_tree** old_data);

int delete_brash(char* buff);

char* make_code(int File);

int delete_enters(char* buff);

char* give_token();

vertex* GetInit(var* array_of_var, func* array_of_func, Tree* mytree);

bool is_stand_func(char* string);

bool is_func(char* string, func* array_of_func);

vertex* GetN(Tree* mytree);

vertex* GetP(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetM(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetExpression (var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetBranching(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetInit(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetString(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetBody(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetG(char* code, var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetRav(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetVar(var* array_of_var, func* array_of_func, Tree* mytree);

vertex* GetCondition(var* array_of_var, func* array_of_func, Tree* mytree);

Tree* code_tree_from_buff(char* code, var* array_of_var, func* array_of_func);

void test (char* in);

int give_num_pointer(char* name, var* array_of_var);

int asm_node(vertex* node, var* array_of_var, func* array_of_func, FILE* out, int num_params);

int Print_asm(FILE* out, Tree* mytree, var* array_of_var, func* array_of_func);

int fill_array_of_func_names(char* code, func* array_of_func);

var* clean_var_array(var* array_of_var, int num_for_cleaning);

int fill_params(var* array_of_var, char* code);

vertex* GetParameter(var* array_of_var, func* array_of_func, Tree* mytree);

int Make_asm_file(var* array_of_var, func* array_of_func, const char* path);

#endif //COMPILATOR_COMPILATOR_H
