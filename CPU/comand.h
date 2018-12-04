

//#ifndef CPU_COMAND_H
//#define CPU_COMAND_H


//#include "Common.h"
#include "DSL_COMANDS.h"
#include "Common.h"

DEF_CMD( PUSH, 65,  {
                        //bin_code[0] = CMD_PUSH;
                        char param1[80] = "";
                        double number = NAN;
                        int num_param = 0;
                        num_param = sscanf(string, "%s %lg", buf, &number);

                        if (num_param == 2)
                        {
                            bin_code[1] = MEM_STK;
                            bin_code[2] = 0;
                            shift = 11;
                            *((double*)(bin_code + 3)) = number;
                            break;
                        }
                        if (sscanf(string, "%s %s", buf, param1) < 2)
                            {
                                printf("Syntax error, after push no param");
                                abort();
                            }

                        if (param1[0] == '[')
                        {
                            bin_code[1] = MEM_RAM;////////
                            if (param1[1] == 'r')
                            {
                                if ((param1[2] < 'a') || (param1[2] > 'd'))
                                {
                                    printf("Errroro - bad register in push [");
                                    abort();
                                }
                                bin_code[2] = param1[2] - 'a' + REG_RAX;
                                shift = 3;
                            }
                            else if (sscanf(string, "%s [%lg]", buf, &number) == 2)
                            {
                                bin_code[2] = 0;
                                shift = 11;
                                *((double*)(bin_code + 3)) = number;
                            }
                            else
                            {
                                printf("Syntax error after [ no register or number");
                                abort();
                            }
                        }
                        else if (param1[0] == 'r')
                        {
                            bin_code[1] = MEM_STK;
                            if ((param1[1] < 'a') || (param1[1] > 'd'))
                            {
                                printf("Errroro - bad register in push");
                                abort();
                            }
                            bin_code[2] = param1[1] - 'a' + REG_RAX;
                            shift = 3;
                        }
                        else
                        {
                            printf("Syntax error: after push no param or [");
                            abort();
                        }
                    }, {})
DEF_CMD(  POP,  66, {
                        //bin_code[0] = CMD_POP;
                        char param1[80] = "";
                        double num_of_cell = NAN;

                        if (sscanf(string, "%s %s", buf, param1) < 2)
                        {
                            bin_code[1] = MEM_STK;
                            bin_code[2] = 0;
                            shift = 3;
                        }
                        else
                        {
                            if (param1[0] == '[')
                            {
                                bin_code[1] = MEM_RAM;
                                if (param1[1] == 'r')
                                {
                                    if ((param1[2] < 'a') || (param1[2] > 'd'))
                                    {
                                        printf("Errroro - bad register in pop [");
                                        abort();
                                    }
                                    bin_code[2] = param1[2] - 'a' + REG_RAX;
                                    shift = 3;
                                }
                                else if (sscanf(string, "%s [%lg]", buf, &num_of_cell) == 2)
                                {
                                    bin_code[2] = 0;
                                    *((double*)(bin_code + 3)) = num_of_cell;
                                    shift = 11;
                                }
                                else
                                {
                                    printf("Syntax error something bad after pop [");
                                    abort();
                                }
                            }
                            else if (param1[0] == 'r')
                            {
                                if ((param1[1] < 'a') || (param1[1] > 'd'))
                                {
                                    printf("Errroro - bad register pop ");
                                    abort();
                                }
                                bin_code[1] = MEM_STK;
                                bin_code[2] = param1[1] - 'a' + REG_RAX;
                                shift = 3;
                            }
                            else
                            {
                                printf("Eroro something bad after pop");
                                abort();
                            }
                        }
                    }, {})
DEF_CMD(  MUL, 67,  { EMPTY_CMD }, {})//C
DEF_CMD(  DIV, 68,  { EMPTY_CMD }, {})//D
DEF_CMD(  ADD, 69,  { EMPTY_CMD }, {})//E
DEF_CMD(  SUB, 70,  { EMPTY_CMD }, {})//F
DEF_CMD(  SIN, 71,  { EMPTY_CMD }, {})//G
DEF_CMD(  COS, 72,  { EMPTY_CMD }, {})//H
DEF_CMD( SQRT, 73,  { EMPTY_CMD }, {})//I
DEF_CMD(   IN, 74,  { EMPTY_CMD }, {})//J
DEF_CMD(  OUT, 75,  { EMPTY_CMD }, {})//K
DEF_CMD(  END, 76,  { EMPTY_CMD }, {})//L
DEF_CMD(  RET, 77,  { EMPTY_CMD }, {})//M
DEF_CMD( CALL, 78,  { JUMPS(CALL) }, {})//N
DEF_CMD(  JMP, 79,  { JUMPS( JMP) }, {})//O
DEF_CMD(   JA, 80,  { JUMPS(  JA) }, {})//P
DEF_CMD(   JB, 81,  { JUMPS(  JB) }, {})//Q
DEF_CMD(   JC, 82,  { JUMPS(  JC) }, {})//R
DEF_CMD(  JAC, 83,  { JUMPS( JAC) }, {})//S
DEF_CMD(  JBC, 84,  { JUMPS( JBC) }, {})//T


DEF_REG( RAX, 97)//a
DEF_REG( RBX, 98)
DEF_REG( RCX, 99)
DEF_REG( RDX, 100)

DEF_MEM( STK, 35)//#
DEF_MEM( RAM, 36)//$

//#endif //CPU_COMAND_H
