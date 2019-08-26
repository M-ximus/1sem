//
// Created by max_l on 16.12.2018.
//

#ifndef DIFFERENCIATOR_BDSL_H
#define DIFFERENCIATOR_BDSL_H

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

#define TYPE node->data->type

#define DATA node->data

#define CREATE create_vertex(out_tree, nullptr, nullptr);

#define LS node->left_son

#define RD node->right_daughter

#define ISNUM_L (node->left_son->data->type == Number)

#define ISNUM_R (node->right_daughter->data->type == Number)

#define ISZERO_R (node->right_daughter->data->type == Number && node->right_daughter->data->number == 0)

#define ISZERO_L (node->left_son->data->type == Number && node->left_son->data->number == 0)



#endif //DIFFERENCIATOR_BDSL_H
