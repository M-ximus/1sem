#include "Tree.h"

Tree* Tree_construct()
{
    Tree* myTree = (Tree*) calloc(1, sizeof(*myTree));
    if (myTree == nullptr)
        return nullptr;

    myTree->root    = nullptr;
    myTree->error   = 0;
    myTree->counter = 0;

    return myTree;
}

vertex* create_left_son(Tree* myTree, vertex* new_parent, data_t* info)
{
    ASSERT(Tree_ok(myTree));
    assert(info != nullptr);

    vertex* new_elem = create_empty_elem(myTree);
    if (new_elem == nullptr)
        return nullptr;

    size_t size = strlen(info) + 1;
    (new_elem->data) = (data_t*) calloc(size, sizeof(*(new_elem->data)));
    if(new_elem->data == nullptr)
    {
        myTree->error = Memory_error;
        free(new_elem);
        return nullptr;
    }

    strcpy(new_elem->data, info);

    if(myTree->root == nullptr)
    {
        myTree->error = Root_error;
        free(new_elem->data);
        free(new_elem);
        return nullptr;
    }

    new_elem->left_son       = new_parent->left_son;
    new_elem->right_daughter = nullptr;
    new_elem->parent         = new_parent;
    new_parent->left_son     = new_elem;

    ASSERT(Tree_ok(myTree));
    return new_elem;
}

vertex* create_right_daughter(Tree* myTree, vertex* new_parent, const data_t* info)////change
{
    ASSERT(Tree_ok(myTree));
    assert(info != nullptr);

    vertex* new_elem = create_empty_elem(myTree);
    if (new_elem == nullptr)
        return nullptr;

    size_t size = strlen(info) + 1;
    (new_elem->data) = (data_t*) calloc(size, sizeof(*(new_elem->data)));
    if(new_elem->data == nullptr)
    {
        myTree->error = Memory_error;
        free(new_elem);
        return nullptr;
    }

    strcpy(new_elem->data, info);

    if(myTree->root == nullptr)
    {
        myTree->error = Root_error;
        free(new_elem->data);
        free(new_elem);
        return nullptr;
    }

    new_elem->left_son         = nullptr;
    new_elem->right_daughter   = new_parent->right_daughter;
    new_elem->parent           = new_parent;
    new_parent->right_daughter = new_elem;

    ASSERT(Tree_ok(myTree));
    return new_elem;
}

vertex* change_root_left(Tree* myTree)////////////////////////////////////////////////change
{
    ASSERT(Tree_ok(myTree));

    if (myTree->root == nullptr)
    {
        myTree->error = Root_error;
        return nullptr;
    }

    vertex* new_root = create_empty_elem(myTree);
    if (new_root == nullptr)
        return nullptr;

    new_root->right_daughter = nullptr;
    new_root->left_son       = myTree->root;
    myTree->root->parent     = new_root;
    myTree->root             = new_root;

    ASSERT(Tree_ok(myTree));

    return new_root;
}

vertex* change_root_right(Tree* myTree)
{
    ASSERT(Tree_ok(myTree));

    if (myTree->root == nullptr)
    {
        myTree->error = Root_error;
        return nullptr;
    }

    vertex* new_root = create_empty_elem(myTree);
    if (new_root == nullptr)
        return nullptr;

    new_root->right_daughter = myTree->root;
    new_root->left_son       = nullptr;
    myTree->root->parent     = new_root;
    myTree->root             = new_root;

    ASSERT(Tree_ok(myTree));

    return new_root;
}

vertex* root_create(Tree* myTree)
{
    ASSERT(Tree_ok(myTree));

    if (myTree->root != nullptr)
    {
        myTree->error = Root_error;
        return nullptr;
    }

    vertex* new_root = create_empty_elem(myTree);
    myTree->root     = new_root;

    ASSERT(Tree_ok(myTree));

    return new_root;
}

bool delete_branch(Tree* myTree, vertex* Judah)
{
    ASSERT(Tree_ok(myTree));

    if (Judah == nullptr)
        return true;

    if (Judah->left_son != nullptr)
        delete_branch(myTree, Judah->left_son);
    if (Judah->right_daughter != nullptr)
        delete_branch(myTree, Judah->right_daughter);

    if (Judah->data != nullptr)
        free(Judah->data);

    Judah->data = nullptr;//Poison_for_strings;

    if (Judah->parent != nullptr)
    {
        if (Judah->parent->left_son == Judah)
            Judah->parent->left_son = nullptr;
        else
            Judah->parent->right_daughter = nullptr;

        Judah->parent = nullptr;
    }

    free(Judah);
    myTree->counter--;

    return true;
}

bool Tree_ok(Tree* myTree)
{
    if (myTree == nullptr)
        return false;

    if (myTree->counter == 0 && myTree->root != nullptr)
    {
        myTree->error = Root_error;
        return false;
    }

    if (myTree->counter != 0 && myTree->root == nullptr)
    {
        myTree->error = Counter_error;
        return false;
    }

    if (myTree->error != 0)
        return false;

    if (myTree->counter != 0)
    {
        if (!(Linkers_ok(myTree->root)))
            return false;
    }

    return true;
}

vertex* create_vertex(Tree* myTree, vertex* left_vertex, vertex* right_vertex, data_t* info)
{
    ASSERT(Tree_ok(myTree));
    //assert(info != nullptr);

    vertex* new_vertex = create_empty_elem(myTree);
    if (new_vertex == nullptr)
        return nullptr;

    new_vertex->right_daughter = right_vertex;
    new_vertex->left_son       = left_vertex;

    if (info == nullptr)
        new_vertex->data = nullptr;
    else
        new_vertex->data = put_info(info);

    if(left_vertex != nullptr && right_vertex != nullptr)
    {
        left_vertex->parent  = new_vertex;
        right_vertex->parent = new_vertex;
    }

    ASSERT(Tree_ok(myTree));

    return new_vertex;
}

bool Tree_distruct(Tree** myTree)
{
    ASSERT(Tree_ok(*myTree));
    if (*myTree == nullptr)
        return false;
    if ((*myTree)->error != 0)
        return false;

    if (!delete_branch(*myTree, (*myTree)->root))
        return false;

    (*myTree)->root = nullptr;

    free(*myTree);
    *myTree = nullptr;

    return  true;
}

data_t* put_info(data_t* info)
{
    assert(info != nullptr);

    size_t size = strlen(info) + 1;

    if (size == 0)
        return nullptr;

    data_t* data = (data_t*) calloc(size, sizeof(*data));

    if (data != nullptr)
        strcpy(data, info);

    return data;
}

vertex* create_empty_elem(Tree* myTree)
{
    ASSERT(Tree_ok(myTree));

    vertex* new_elem = (vertex*) calloc(1, sizeof(*new_elem));
    if (new_elem == nullptr)
    {
        myTree->error = Memory_error;
        return nullptr;
    }

    new_elem->data           = nullptr;//Poison_for_new_strings;
    new_elem->parent         = nullptr;
    new_elem->left_son       = nullptr;
    new_elem->right_daughter = nullptr;
    myTree->counter++;

    return new_elem;
}

bool check_links(vertex* node)
{
    if (node == nullptr)
        return false;

    if (node->left_son != nullptr)
    {
        if (node->left_son->parent != node)
            return false;
    }
    if (node->right_daughter != nullptr)
    {
        if (node->right_daughter->parent != node)
            return false;
    }

    return true;
}

bool Linkers_ok(vertex* node)
{
    if (node == nullptr)
        return false;

    if (!(check_links(node)))
        return false;

    if (node->left_son != nullptr)
    {
        if (!(Linkers_ok(node->left_son)))
            return false;
    }

    if (node->right_daughter != nullptr)
    {
        if (!(Linkers_ok(node->right_daughter)))
            return false;
    }

    if (node->right_daughter == nullptr && node->left_son == nullptr)
        return true;

    return true;
}

bool save_tree(vertex* node, FILE* outFile)
{
    assert(outFile != nullptr);

    if (node == nullptr)
        return false;

    static int num_tabs = 0;

    fprintf(outFile, "%*s%s\n", num_tabs*4, "{", node->data);
    num_tabs++;

    save_tree(node->left_son, outFile);
    save_tree(node->right_daughter, outFile);

    num_tabs--;

    fprintf(outFile, "%*s\n", num_tabs*4, "}");
    return true;
}

Tree* create_tree_from_file(mystring* strings)
{
    assert(strings != nullptr);

    Tree* myTree = Tree_construct();
    root_create(myTree);

    counters num_string = {0, 0};

    myTree->root = create_branch(myTree, strings, &num_string);

    return myTree;
}

vertex* create_branch(Tree* myTree, mystring* strings, counters* num_string)
{
    assert(strings != nullptr);
    assert(myTree != nullptr);

    if (num_string->size == 0)
    {
        if (strings[num_string->size].String[0] != '{')
            return nullptr;
        myTree->root->data = put_info(strings[num_string->size].String + 1);
        (num_string->size)++;
        myTree->root->left_son = create_branch(myTree, strings, num_string);
        myTree->root->left_son->parent =  myTree->root;
        myTree->root->right_daughter = create_branch(myTree, strings, num_string);
        myTree->root->right_daughter->parent = myTree->root;
        return myTree->root;
    }

    if (strings[num_string->size].String[0] == '{')
    {
        vertex* node = create_vertex(myTree, nullptr, nullptr, strings[num_string->size].String + 1);// without string because we don't want fragmentation of memory

        if (strings[num_string->size + 1].String[0] == '}')
        {
            int pos = num_string->size + 1;
            while(strings[pos].String != nullptr && strings[pos].String[0] == '}')
                pos++;
            num_string->size = pos;

            return node;
        }
        else
        {
            (num_string->size)++;
            node->left_son = create_branch(myTree, strings, num_string);
            node->left_son->parent = node;
            node->right_daughter = create_branch(myTree, strings, num_string);
            node->right_daughter->parent = node;
            return node;
        }

    }

    return nullptr;
}

mystring* create_array_of_strings_fromFile(int inFile)
{
    assert(inFile > 0);

    char* buff = fillBuff(inFile);
    assert(buff != nullptr);

    mystring* addressesStrings = fillAddress(buff);

    assert(addressesStrings != nullptr);

    return addressesStrings;
}

char* fillBuff(int inFile)
{
    assert(inFile > 0);

    long int numSymbols = calcSize(way_to_in);
    assert(numSymbols != 0);

    char* buff = (char*) calloc(numSymbols, sizeof(*buff));
    assert(buff != nullptr);

    long int error = read(inFile, buff, numSymbols);
    assert(error == numSymbols);

    return buff;
}

long int calcSize(const char* inFile)
{
    assert(inFile != nullptr);

    struct _stat fileInfo;
    _stat(inFile, &fileInfo);

    return fileInfo.st_size;
}

size_t calc_number_of_special_symbol(char* buff, char symbol)
{
    assert(buff != nullptr);

    size_t numSymbols = 1;
    for(char* pos = strchr(buff, symbol);pos;pos = strchr(pos + 1, symbol))
        numSymbols++;

    return numSymbols;
}

mystring* fillAddress(char* buff)
{
    assert(buff != nullptr);

    size_t numStrings = calc_number_of_special_symbol(buff, '\n');
    assert(numStrings > 0);

    mystring* addressesStrings = (mystring*) calloc(numStrings + 1, sizeof(*addressesStrings));
    addressesStrings[0].String = buff;

    int pos_in_addressesStrings = 1;
    char* first_pos = buff;
    for(int i = 1; buff[i] != '\0' && pos_in_addressesStrings < numStrings; i++)// j - pos
    {
        if (buff[i] == '\r')
        {
            char* open_scope = strchr(buff + i, '{');
            char* close_scope = strchr(buff + i, '}');
            if (open_scope != nullptr && close_scope != nullptr)
            {
                if (open_scope < close_scope)
                    addressesStrings[pos_in_addressesStrings].String = open_scope;
                else
                    addressesStrings[pos_in_addressesStrings].String = close_scope;
            }
            else
                addressesStrings[pos_in_addressesStrings].String = close_scope;
            addressesStrings[pos_in_addressesStrings - 1].len = buff + i + 1 - first_pos;
            first_pos = addressesStrings[pos_in_addressesStrings].String;
            pos_in_addressesStrings++;
            buff[i] = '\0';
        }
    }
    addressesStrings[numStrings].String = nullptr;

    return addressesStrings;
}

void destroyer(char* buff, mystring* addressesStrings)
{
    free(buff);
    free(addressesStrings);
}

size_t calc_no_zero_Addresses(mystring* addressesStrings)
{
    assert(addressesStrings != nullptr);

    size_t size = 1;
    for(; addressesStrings[size].String != nullptr; size++);
    return size;
}

void Print_tree(mystring* Code)
{
    assert(Code != nullptr);

    size_t i = 0;
    while(Code[i].String != nullptr)
    {
        printf("%s\n", Code[i].String);
        i++;
    }
}

vertex* asking(vertex* node)
{
    //vertex* current_node = myTree->root;
    vertex* founded = node;
    if (node != nullptr && node->right_daughter != nullptr) {
        char answer = 0;
        printf("Это %s yes[y] / no[n]\n", node->data);
        scanf("\n%c", &answer);
        if (answer == 'n')
            founded = asking(node->right_daughter);
        else
            founded = asking(node->left_son);
    }

    return founded;
}

vertex* push_new(vertex* node, Tree* myTree)
{
    ASSERT(Tree_ok(myTree));
    assert(node != nullptr);

    printf("Я не угадал?! Живи... Какое хоть слово загадал?\n");
    char new_word[1000] = "";
    scanf("\n");
    gets(new_word);

    printf("Воу. А чем %s отличается %s?\n", new_word, node->data);
    char new_question[1001] = "";
    scanf("\n");
    gets(new_question);
    strcat(new_question, "?");

    create_left_son(myTree, node, new_word);
    create_right_daughter(myTree, node, node->data);

    free(node->data);
    node->data = put_info(new_question);

    ASSERT(Tree_ok(myTree));
    return node;
}