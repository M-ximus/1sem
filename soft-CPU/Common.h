

#ifndef CPU_SOURSE_H
#define CPU_SOURSE_H

enum{
    #define DEF_CMD(name, num_arg, func_asm, func_cpu) CMD_##name = (num_arg),
    #define DEF_REG(name, num_arg) REG_##name = (num_arg),
    #define DEF_MEM(name, num_arg) MEM_##name = (num_arg),

    #include "comand.h"

    #undef DEF_CMD
    #undef DEF_REG
    #undef DEF_MEM
};

#endif //CPU_SOURSE_H
