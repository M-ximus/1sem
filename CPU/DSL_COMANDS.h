//
// Created by max_l on 03.12.2018.
//

#ifndef CPU_DSL_COMANDS_H
#define CPU_DSL_COMANDS_H

#define EMPTY_CMD {\
    bin_code[1] = MEM_STK;\
    bin_code[2] = 0;\
    shift = 3;\
};

#define COMPILATION_WITHOUT_LABELS {\
    bin_code[1] = -1;\
    bin_code[2] = -1;\
    *((double*)(bin_code + 3)) = NAN;\
    shift = 11;\
    };

#define JUMPS( command ) {\
    if (num_compilation == 0)\
    {\
        COMPILATION_WITHOUT_LABELS\
    }\
    else\
    {\
        char name_of_label[80] = "";\
        if (sscanf(string, "%s :%s", buf, name_of_label) < 2)\
        {\
            printf("Errororo in %s", #command);\
            abort();\
        }\
        int pos_for_jump = search_in_labels(name_of_label, labels, *free_label);\
        if (pos_for_jump == -1)\
        {\
            printf("Errororroror I can't find this label - %s for %s", name_of_label, #command);\
            abort();\
        }\
        bin_code[1] = MEM_STK;\
        bin_code[2] = 0;\
        *((double*)(bin_code + 3)) = (double) pos_for_jump;\
        shift = 11;\
    }\
};

#define CHECK_REG {\
    if (bynary_code[*pc] < REG_RAX || bynary_code[*pc] >= (REG_RAX + num_of_reg))\
    {\
        printf("Binary code error, i don't know this register %c", bynary_code[*pc]);\
        abort();\
    }\
};

#define CUR_REG (bynary_code[*pc] - REG_RAX)

#define EMPTY_SHIFT *pc += 2;

#define POP_TWO_NUMBERS \
    first_element = stackPop(CPU_stack);\
    if (!std::isfinite(first_element))\
    {\
        printf("Pop from zero-stack!!!");\
        abort();\
    }\
    second_element = stackPop(CPU_stack);\
    if (!std::isfinite(first_element))\
    {\
        printf("Pop from zero-stack!!!");\
        abort();\
    }

#define POP_ONE_NUMBER \
    first_element = stackPop(CPU_stack);\
    if (!std::isfinite(first_element))\
    {\
        printf("Pop from zero-stack!!!");\
        abort();\
    }

#define POS_TO_JUMP \
    double* data = (double*) (bynary_code + *pc);\
    int pos_to_jump = (int) (*data);

#endif //CPU_DSL_COMANDS_H
