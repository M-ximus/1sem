#include "Tree.h"

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int inFile = open(way_to_in, O_RDONLY|O_BINARY);
    assert(inFile != -1);

    mystring* addressesStrings = create_array_of_strings_fromFile(inFile);
    assert(addressesStrings != nullptr);
    close(inFile);

    char* buff = addressesStrings[0].String;
    Print_tree(addressesStrings);
    Tree* test = create_tree_from_file(addressesStrings);
    destroyer(buff, addressesStrings);

    char ans = 0;
    vertex* current = nullptr;
    printf("Не хочешь сыграть в игру? Если я выиграю, то ты умрешь \nyes[y] / no[n]\n");
    while(scanf("\n%c", &ans) && ans != 'n')
    {
        current = asking(test->root);
        char answer = 0;
        if (current != nullptr)
        {
            printf("Ты загадал %s? yes[y] / no[n]\n", current->data);
            if (scanf("\n%c", &answer) && answer == 'n')
                push_new(current, test);
            else
                printf("Ты умер\nМяу =^..^=\n");
        }
        printf("Хочешь еще сыграть? yes[y] / no[n]\n");
    }

    printf("Беги, форест, беги");
    FILE* outFile = fopen(way_to_out, "w");
    assert(outFile != nullptr);

    save_tree(test->root, outFile);

    fclose(outFile);

    Tree_distruct(&test);

    return 0;
}