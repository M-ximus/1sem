//#ifndef COMPILATOR_STANDART_FUNC_LIB_H
//#define COMPILATOR_STANDART_FUNC_LIB_H

DEF_SFL( sqrt,  {
                    asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
                    fprintf(out, "SQRT\n");
                });
DEF_SFL( Luke,  {
                    asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
                    fprintf(out, "OUT\n");
                });
DEF_SFL( Chui,  {
                    fprintf(out, "END\n");
});
DEF_SFL( Leia,  {
                    fprintf(out, "IN\n");
                    int pointer = give_num_pointer(node->right_daughter->left_son->data->var_name, array_of_var);
                    assert(pointer <= num_of_var);

                    fprintf(out, "POP [rbx + %d]\n", pointer);
                    });

//#endif //COMPILATOR_STANDART_FUNC_LIB_H
