

#ifndef COMPILATOR_BDSL_H
#define COMPILATOR_BDSL_H

#define RESTRUCT_OPER {\
    vertex* new_op = create_vertex(mytree, nullptr, nullptr);\
    assert(new_op != nullptr);\
    \
    new_op->left_son        = new_node->left_son;\
    new_op->right_daughter  = new_node->right_daughter;\
    new_op->parent          = new_node;\
    new_op->data->type      = Operator;\
    new_op->data->oper_name = new_node->data->oper_name;\
    new_node->left_son->parent = new_op;\
    new_node->right_daughter->parent = new_op;\
    new_node->left_son      = new_op;\
};

#endif //COMPILATOR_BDSL_H
