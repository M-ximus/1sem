

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
                    },  {
                            if (bynary_code[*pc] == MEM_STK)
                            {
                                (*pc)++;
                                if (bynary_code[*pc] != 0)
                                {
                                    CHECK_REG
                                    stackPush(CPU_stack, REGisters[CUR_REG]);
                                    (*pc)++;
                                }
                                else
                                {
                                    (*pc)++;
                                    double* number_from_code = (double*) (bynary_code + (*pc));
                                    stackPush(CPU_stack, *number_from_code);
                                    (*pc) += 8;
                                }
                            }
                            else if (bynary_code[*pc] == MEM_RAM)
                            {
                                (*pc)++;
                                if (bynary_code[*pc] != 0)
                                {
                                    CHECK_REG
                                    int cur_address = (int) REGisters[CUR_REG];
                                    if (cur_address <= 0 || cur_address >= size_of_RAM)
                                    {
                                        printf("Error, bad address in RAM - %d", cur_address);
                                        abort();
                                    }
                                    stackPush(CPU_stack, RAM[cur_address]);
                                    (*pc)++;
                                }
                                else
                                {
                                    (*pc)++;
                                    double* number_from_code = (double*) (bynary_code + (*pc));
                                    int cur_address = (int) (*number_from_code);
                                    if (cur_address <= 0 || cur_address >= size_of_RAM)
                                    {
                                        printf("Erorororororo, bad address of RAM - %d", cur_address);
                                        abort();
                                    }
                                    stackPush(CPU_stack, RAM[cur_address]);
                                    (*pc) += 8;
                                }
                            }
                            else
                            {
                                printf("I don't know this type of memory - %c", bynary_code[*pc]);
                                abort();
                            }
                        })
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
                    },  {
                            if (bynary_code[*pc] == MEM_STK)
                            {
                                (*pc)++;
                                if (bynary_code[*pc] != 0)
                                {
                                    CHECK_REG
                                    REGisters[CUR_REG] = stackPop(CPU_stack);
                                    (*pc)++;
                                }
                                else
                                {
                                    (*pc)++;
                                }
                            }
                            else if (bynary_code[*pc] == MEM_RAM)
                            {
                                (*pc)++;
                                if (bynary_code[*pc] != 0)
                                {
                                    CHECK_REG
                                    int cur_address = (int) REGisters[CUR_REG];
                                    if (cur_address <= 0 || cur_address >= size_of_RAM)
                                    {
                                        printf("Error, bad address in RAM - %d", cur_address);
                                        abort();
                                    }
                                    RAM[cur_address] = stackPop(CPU_stack);
                                    (*pc)++;
                                }
                                else
                                {
                                    (*pc)++;
                                    double* number_from_code = (double*) (bynary_code + (*pc));
                                    int cur_address = (int) (*number_from_code);
                                    if (cur_address <= 0 || cur_address >= size_of_RAM)
                                    {
                                        printf("Erorororororo, bad address of RAM - %d", cur_address);
                                        abort();
                                    }
                                    RAM[cur_address] = stackPop(CPU_stack);
                                    (*pc) += 8;
                                }
                            }
                            else
                            {
                                printf("I don't know this type of memory - %c", bynary_code[*pc]);
                                abort();
                            }
                        })
DEF_CMD(  MUL, 67,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        double second_element = NAN;
                                        POP_TWO_NUMBERS

                                        stackPush(CPU_stack, first_element * second_element);
                                    })//C
DEF_CMD(  DIV, 68,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        double second_element = NAN;
                                        POP_TWO_NUMBERS

                                        stackPush(CPU_stack, second_element / first_element);
                                    })//D
DEF_CMD(  ADD, 69,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        double second_element = NAN;
                                        POP_TWO_NUMBERS

                                        stackPush(CPU_stack, second_element + first_element);
                                    })//E
DEF_CMD(  SUB, 70,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        double second_element = NAN;
                                        POP_TWO_NUMBERS

                                        stackPush(CPU_stack, second_element - first_element);
                                    })//F
DEF_CMD(  SIN, 71,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        POP_ONE_NUMBER

                                        stackPush(CPU_stack, sin(first_element));
                                    })//G
DEF_CMD(  COS, 72,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        POP_ONE_NUMBER

                                        stackPush(CPU_stack, cos(first_element));
                                    })//H
DEF_CMD( SQRT, 73,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = NAN;
                                        POP_ONE_NUMBER

                                        stackPush(CPU_stack, sqrt(first_element));
                                    })//I
DEF_CMD(   IN, 74,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = 0;
                                        if (!scanf("%lg", &first_element))
                                        {
                                            printf("You didn't put number");
                                            abort();
                                        }

                                        stackPush(CPU_stack, first_element);
                                    })//J
DEF_CMD(  OUT, 75,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double cur = stackPop(CPU_stack);
                                        printf("%lg", cur);
                                    })//K
DEF_CMD(  END, 76,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                    })//L
DEF_CMD(  RET, 77,  { EMPTY_CMD },  {
                                        EMPTY_SHIFT
                                        double first_element = stackPop(CALL_stack);
                                        if (!std::isfinite(first_element))
                                        {
                                            printf("You put into CALL stack smth bad or CALL stack is empty");
                                            abort();
                                        }
                                        (*pc) = (int) first_element;
                                    })//M

DEF_CMD( CALL, 78,  { JUMPS(CALL) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP

                                            double pos_to_return = (double) (*pc + 8);
                                            stackPush(CALL_stack, pos_to_return);

                                            (*pc) = pos_to_jump;
                                        })//N
DEF_CMD(  JMP, 79,  { JUMPS( JMP) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            *pc = pos_to_jump;
                                        })//O
DEF_CMD(   JA, 80,  { JUMPS(  JA) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            double first_element = NAN;
                                            double second_element = NAN;
                                            POP_TWO_NUMBERS

                                            if (second_element > first_element && !is_equal(first_element, second_element))
                                                (*pc) = pos_to_jump;
                                            else
                                                (*pc) += 8;

                                        })//P
DEF_CMD(   JB, 81,  { JUMPS(  JB) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            double first_element = NAN;
                                            double second_element = NAN;
                                            POP_TWO_NUMBERS

                                            if (second_element < first_element && !is_equal(first_element, second_element))
                                                (*pc) = pos_to_jump;
                                            else
                                                (*pc) += 8;
                                        })//Q
DEF_CMD(   JC, 82,  { JUMPS(  JC) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            double first_element = NAN;
                                            double second_element = NAN;
                                            POP_TWO_NUMBERS

                                            if (is_equal(first_element, second_element))
                                                (*pc) = pos_to_jump;
                                            else
                                                (*pc) += 8;
                                        })//R
DEF_CMD(  JAC, 83,  { JUMPS( JAC) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            double first_element = NAN;
                                            double second_element = NAN;
                                            POP_TWO_NUMBERS

                                            if (second_element > first_element || is_equal(first_element, second_element))
                                                (*pc) = pos_to_jump;
                                            else
                                                (*pc) += 8;
                                        })//S
DEF_CMD(  JBC, 84,  { JUMPS( JBC) },    {
                                            EMPTY_SHIFT
                                            POS_TO_JUMP
                                            double first_element = NAN;
                                            double second_element = NAN;
                                            POP_TWO_NUMBERS

                                            if (second_element > first_element || is_equal(first_element, second_element))
                                                (*pc) = pos_to_jump;
                                            else
                                                (*pc) += 8;
                                        })//T
DEF_CMD(  SPC, 85,  { EMPTY_CMD },      {
                                            EMPTY_SHIFT
                                            printf(" ");
                                        })
DEF_CMD( MEOW, 86,  { EMPTY_CMD },      {
                                            EMPTY_SHIFT
                                            printf("\n                                               .--.\n"
"                                               `.  \\\n"
"                                                 \\  \\\n"
"                                                  .  \\\n"
"                                                  :   .\n"
"                                                  |    .\n"
"                                                  |    :\n"
"                                                  |    |\n"
"  ..._  ___                                       |    |\n"
" `.\"\".`''''\"\"--..___                              |    |\n"
" ,-\\  \\             \"\"-...__         _____________/    |\n"
" / ` \" '                    `\"\"\"\"\"\"\"\"                  .\n"
" \\                                                      L\n"
" (>                                                      \\\n"
"/                                                         \\\n"
"\\_    ___..---.                                            L\n"
"  `--'         '.                                           \\\n"
"                 .                                           \\_\n"
"                _/`.                                           `.._\n"
"             .'     -.                                             `.\n"
"            /     __.-Y     /''''''-...___,...--------.._            |\n"
"           /   _.\"    |    /                ' .      \\   '---..._    |\n"
"          /   /      /    /                _,. '    ,/           |   |\n"
"          \\_,'     _.'   /              /''     _,-'            _|   |\n"
"                  '     /               `-----''               /     |\n"
"                  `...-'     for Ded)                          !_____)");
})


DEF_REG( RAX, 97)//a
DEF_REG( RBX, 98)
DEF_REG( RCX, 99)
DEF_REG( RDX, 100)

DEF_MEM( STK, 35)//#
DEF_MEM( RAM, 36)//$

//#endif //CPU_COMAND_H
