#include "Compilator.h"

int main() {
    int input = open(way_to_in, O_RDONLY|O_BINARY);
    assert(input >= 0);
    char* code = make_code(input);
    close(input);
    assert(code != nullptr);

    //test(code);

    func* array_of_func = (func*) calloc(num_of_func, sizeof(*array_of_func));
    var* array_of_var = (var*) calloc(num_of_var, sizeof(*array_of_var));

    fill_array_of_func_names(code, array_of_func);

    Make_asm_file(array_of_var, array_of_func, way_to_out);

    //Tree* fake_code = code_tree_from_buff(array_of_func[1].code, array_of_var, array_of_func);
    //ASSERT(Tree_ok(fake_code));

    //tree_dot(fake_code, "C:\\Users\\max_l\\CLionProjects\\compilator\\Dot.dot");

    //Print_asm(way_to_out, fake_code, array_of_var, array_of_func);

    //Tree_distruct(&fake_code);
    free(code);
    return 0;
}