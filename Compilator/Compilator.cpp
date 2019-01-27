#include "Compilator.h"
#include "BDSL.h"

char* pos = nullptr;

void test (char* in)
{
    pos = in;
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
    printf("%s\n", give_token());
}

int Make_asm_file(var* array_of_var, func* array_of_func, const char* path)
{
    FILE* out_file = fopen(path, "w");
    fprintf(out_file, "PUSH 1\n");
    fprintf(out_file, "POP rbx\n");
    fprintf(out_file, "PUSH 0\n");
    fprintf(out_file, "POP rcx\n");
    fprintf(out_file, "CALL :In_the_far_far_away_galaxy\n");
    fprintf(out_file, "END\n");

    for(int i = 0; i < num_of_func; i++)
    {
        if (array_of_func[i].name_of_func == nullptr)
            continue;
        Tree* fake = Tree_construct();
        fake = code_tree_from_buff(array_of_func[i].code, array_of_var, array_of_func);
        ASSERT(Tree_ok(fake));
        tree_dot(fake, "C:\\Users\\max_l\\CLionProjects\\compilator\\Dot.dot");
        fprintf(out_file, ":%s\n", array_of_func[i].name_of_func);
        asm_node(fake->root, array_of_var, array_of_func, out_file, array_of_func[i].num_of_param);
    }
    //fprintf(out_file,";");

    return 0;
}

/*int Print_asm(FILE* out, Tree* mytree, var* array_of_var, func* array_of_func)
{
    assert(mytree != nullptr);
    assert(array_of_var != nullptr);
    assert(array_of_func != nullptr);
    assert(out != nullptr);

    asm_node(mytree->root, array_of_var, array_of_func, out);
    //fprintf(out, "END");

    fclose(out);
    return 1;
}*/

int fill_array_of_func_names(char* code, func* array_of_func)
{
    assert(code != nullptr);
    assert(array_of_func != nullptr);

    int pos_in_array = 0;
    char* pointer_to_func = strchr(code, '#');
    if (pointer_to_func == nullptr)
    {
        printf("I can't find \"In_the_far_far_away_galaxy\"");
        abort();
    }

    while (pointer_to_func != nullptr)
    {
        pos = pointer_to_func + 1;
        char* name = give_token();
        assert(name != nullptr);

        array_of_func[pos_in_array].name_of_func = name;
        array_of_func[pos_in_array].code = pos;
        char* cur = give_token();
        assert(cur[0] == '(');

        int num_of_param = 0;
        while (cur[0] != ')')
        {
            free(cur);
            cur = give_token();
            if (isalpha(cur[0]))
            {
                num_of_param++;
                free(cur);
                cur = give_token();
            }
            else if (cur[0] == ',')
            {
                    free(cur);
                    cur = give_token();
                    continue;
            }
            else if (cur[0] == ')')
                break;
            else
            {
                printf("I can't calc this param");
                abort();
            }
        }

        pointer_to_func = strchr(pos, '#');
        free(cur);
        array_of_func[pos_in_array].num_of_param = num_of_param;
        pos_in_array++;
        if (pos_in_array == num_of_func)
        {
            printf("Tooo much functions");
            abort();
        }

    }
}

int calc_var(var* array_of_var)
{
    assert(array_of_var != nullptr);

    int answer = 0;
    for(int i = 0; i < num_of_var; i++)
    {
        if (array_of_var[i].name_of_var == nullptr)
            break;
        answer++;
    }
    return answer;
}

int asm_node(vertex* node, var* array_of_var, func* array_of_func, FILE* out, int num_params)
{
    if (node == nullptr)
        return 0;

    if (node->parent == nullptr)
    {
        for(int i = num_params - 1; i >= 0; i--)
            fprintf(out, "POP [rbx + %d]\n", i);
    }

    static int label_for_branching = 0;

    if (node->data->type == Body)
    {
        assert(node->left_son != nullptr);
        asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
        if (node->right_daughter == nullptr)
            return 0;
        else
            asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
        return 1;
    }

    switch (node->data->type)
    {
        case Number:
        {
            fprintf(out, "PUSH %d\n", node->data->number);
            break;
        }
        case Equality:
        {
            asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
            assert(node->left_son != nullptr);

            int pointer = give_num_pointer(node->left_son->data->var_name, array_of_var);
            assert(pointer <= num_of_var);

            fprintf(out, "POP [rbx + %d]\n", pointer);
            break;
        }
        case Variable:
        {
            int pointer = give_num_pointer(node->data->var_name, array_of_var);
            assert(pointer <= num_of_var);

            fprintf(out, "PUSH [rbx + %d]\n", pointer);
            break;
        }
        case Inicialise:
            break;
        case Condition:
        {
            asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
            asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
            if (!strcmp(node->data->oper_name, "<"))
                fprintf(out, "JAC :");
            else if (!strcmp(node->data->oper_name, ">"))
                fprintf(out, "JBC :");
            else if (!strcmp(node->data->oper_name, "="))
                fprintf(out, "JNC :");
            else if (!strcmp(node->data->oper_name, "!="))
                fprintf(out, "JC :");

            break;
        }
        case Operator:
        {
            asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
            asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
            if (!strcmp(node->data->oper_name, "+"))
                fprintf(out, "ADD\n");
            else if (!strcmp(node->data->oper_name, "-"))
                fprintf(out, "SUB\n");
            else if (!strcmp(node->data->oper_name, "*"))
                fprintf(out, "MUL\n");
            else if (!strcmp(node->data->oper_name, "/"))
                fprintf(out, "DIV\n");

            break;
        }
        case Function:
        {
            fprintf(out, "PUSH rbx\n");
            fprintf(out, "PUSH %d\n", calc_var(array_of_var));
            fprintf(out, "ADD\n");
            fprintf(out, "POP rbx\n");
            fprintf(out, "PUSH %d\n", calc_var(array_of_var));
            fprintf(out, "POP rcx\n");
            asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
            fprintf(out, "CALL :%s\n", node->data->func_name);
            fprintf(out, "PUSH rax\n");
            break;
        }
        case Standart_func:
        {
            #define DEF_SFL(name, asm_function) if (!strcmp(node->data->stand_func_name, #name)) {asm_function; break;}

            #include "Standart_func_lib.h"

            #undef DEF_SFL

            printf("problems with standart lib(((");
            abort();
            break;
        }
        case Branching:
        {
            if (!strcmp(node->data->branch_name, "while"))
            {
                int cur_num = label_for_branching;
                label_for_branching++;
                fprintf(out, ":label_while_%d\n", cur_num);
                asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
                fprintf(out, "label_while_out%d\n", cur_num);
                asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
                fprintf(out, "JMP :label_while_%d\n", cur_num);
                fprintf(out, ":label_while_out%d\n", cur_num);
            }
            else if(!strcmp(node->data->branch_name, "it's_your_droids?"))
            {
                int cur_num = label_for_branching;
                label_for_branching++;
                asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
                fprintf(out, "label_if%d\n", cur_num);
                asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
                fprintf(out, ":label_if%d\n", cur_num);

            }
            else
            {
                printf("Error with branching");
                abort();
            }
            break;
        }
        case Return:
        {
            asm_node(node->left_son, array_of_var, array_of_func, out, num_params);
            fprintf(out, "POP rax\n");
            fprintf(out, "PUSH rbx\n");
            fprintf(out, "PUSH rcx\n");
            fprintf(out, "SUB\n");
            fprintf(out, "POP rbx\n");
            fprintf(out, "RET\n");
        }
        case Parameter:
        {
            assert(node->left_son != nullptr);
            asm_node(node->left_son, array_of_var, array_of_func, out, num_params);

            if (node->right_daughter != nullptr)
                asm_node(node->right_daughter, array_of_var, array_of_func, out, num_params);
            break;
        }
        default:
        {
            printf("Error, %d\n", node->data->type);
        }
    }

    return 0;

}

int give_num_pointer(char* name, var* array_of_var)
{
    assert(name != nullptr);

    int i = 0;
    for(; i < num_of_var; i++)
    {
        if (array_of_var[i].name_of_var == nullptr)
            break;
        if (!strcmp(name, array_of_var[i].name_of_var))
            return array_of_var[i].position;
    }

    return 0;
}

char* make_code(int File)
{
    assert(File >= 0);

    char* buff = fillBuff(File);
    assert(buff != nullptr);

    delete_brash(buff);
    assert(buff != nullptr);

    delete_enters(buff);
    assert(buff != nullptr);

    return buff;
}

int delete_enters(char* buff)
{
    assert(buff != nullptr);

    char* brash = strchr(buff, '\r');
    int i = 0;

    while(brash != nullptr && *(buff + i))
    {
        *brash = ' ';
        *(brash + 1) = ' ';
        i = brash - buff + 2;
        brash = strchr(buff + i, '\r');
    }
    return i;
}

int delete_brash(char* buff)
{
    assert(buff != nullptr);

    int i = 0;
    char* brash = strchr(buff, '/');

    while(brash != nullptr)
    {
        if (*(brash + 1) == '/')
        {
            int shift = 0;
            while(*(brash + shift) != '\r' && *(brash + shift) != '\0')
            {
                *(brash + shift) = ' ';
                shift++;
            }
            i += shift;
        }
        else
            i = brash - buff + 2;
        brash = strchr(buff + i, '/');
    }
    return i;
}

Tree* code_tree_from_buff(char* code, var* array_of_var, func* array_of_func)
{
    assert(code != nullptr);
    assert(array_of_func != nullptr);
    assert(array_of_var != nullptr);

    clean_var_array(array_of_var, num_of_var);
    fill_params(array_of_var, code);

    for (int i = 0; i < num_of_func; i++)
    {
        if (array_of_var[i].name_of_var == nullptr)
            break;
        printf("Name - %s, position = %d\n", array_of_var[i].name_of_var, array_of_var[i].position);
    }

    Tree* function = Tree_construct();
    function->root = GetG(pos, array_of_var, array_of_func, function);

    ASSERT(Tree_ok(function));

    return function;
}

var* clean_var_array(var* array_of_var, int num_for_cleaning)
{
    assert(array_of_var != nullptr);
    assert(num_for_cleaning >= 0);

    for(int i = 0; i < num_for_cleaning; i++)
    {
        if(array_of_var[i].name_of_var == nullptr)
            continue;
        free(array_of_var[i].name_of_var);
        array_of_var[i].name_of_var = nullptr;
    }

    return array_of_var;
}

int fill_params(var* array_of_var, char* code)
{
    assert(code != nullptr);
    assert(array_of_var != nullptr);

    int pos_in_var = 0;
    pos = code;

    char *cur = give_token();
    assert(cur[0] == '(');

    while (cur[0] != ')') {
        free(cur);
        cur = give_token();

        if (isalpha(cur[0])) {
            array_of_var[pos_in_var].name_of_var = put_info(cur);
            array_of_var[pos_in_var].position = pos_in_var;
            pos_in_var++;
        } else if (cur[0] == ',') {
            free(cur);
            continue;
        } else if (cur[0] == ')') {
            free(cur);
            break;
        } else {
            printf("Sorry, but i don't know this");
            abort();
        }
    }
    free(cur);

    return 0;
}

vertex* GetG(char* code, var* array_of_var, func* array_of_func, Tree* mytree)
{
    assert(code != nullptr);
    assert(array_of_func != nullptr);
    assert(array_of_var != nullptr);

    pos = code;
    char* start = give_token();
    CHECK_LEN(code);

    vertex* root = nullptr;

    if(!strcmp(start, "Yoda_said:"))
    {
        char* old_pos = pos;
        root = GetBody(array_of_var, array_of_func, mytree);
        CHECK_LEN(old_pos)
    }
    else
    {
        printf("Bad start of function(((");
        abort();
    }

    char* end = give_token();
    if (strcmp(end, ",may_the_force_be_with_you"))
    {
        printf("Attention!!! I don't know this - %s", end);
        abort();
    }

    free(end);
    free(start);

    return root;
}

vertex* GetBody(var* array_of_var, func* array_of_func, Tree* mytree)
{
    vertex* node = create_vertex(mytree, nullptr, nullptr);

    node->left_son = GetString(array_of_var, array_of_func, mytree);
    node->data->type = Body;
    if (node->left_son == nullptr)
    {
        delete_branch(mytree, node);
        return nullptr;
    }
    node->left_son->parent = node;
    char* old_pos = pos;
    char* token = give_token();
    if (strcmp(token, ",my_young_padawan"))
    {
        printf("Yoda error, he didn't say after string - my young padawan");
        abort();
    }

    node->right_daughter = GetBody(array_of_var, array_of_func, mytree);
    if (node->right_daughter != nullptr)
        node->right_daughter->parent = node;

    return node;
}

vertex* GetString(var* array_of_var, func* array_of_func, Tree* mytree)
{
    vertex* node = nullptr;

    node = GetInit(array_of_var, array_of_func, mytree);
    if (node == nullptr)
    {
        node = GetBranching(array_of_var, array_of_func, mytree);
        if (node == nullptr)
            node = GetRav(array_of_var, array_of_func, mytree);
    }

    return node;
}

vertex* GetInit(var* array_of_var, func* array_of_func, Tree* mytree)
{
    char* old_pos = pos;
    char* name = give_token();
    char* oper = give_token();

    if (!strcmp(oper, ",I'm_your_father"))
    {
        free(oper);
        vertex* node = create_vertex(mytree, nullptr, nullptr);
        node->data->type = Inicialise;
        node->data->var_name = put_info(name);
        int i = 0;

        for(; i < num_of_var; i++)
        {
            if(array_of_var[i].name_of_var == nullptr)
            {
                array_of_var[i].name_of_var = node->data->var_name;
                array_of_var[i].position = i;
                printf("pos of variable %s = %d", node->data->var_name, i);
                break;
            }
        }
        if (i == num_of_var)
        {
            printf("Number of variables > max");
            abort();
        }

        free(name);
        return node;
    }

    pos = old_pos;
    free(name);
    free(oper);

    return nullptr;
}

vertex* GetBranching(var* array_of_var, func* array_of_func, Tree* mytree)
{
    char* old_pos = pos;

    char* maybe_body = give_token();

    if (maybe_body[0] == '{')
    {
        vertex* branch_func = create_vertex(mytree, nullptr, nullptr);
        branch_func->left_son = GetBody(array_of_var, array_of_func, mytree);
        char* end = give_token();
        if (end[0] != '}')
        {
            printf("Bad end of body");
            abort();
        }
        branch_func->left_son->parent = branch_func;
        branch_func->data->type = Branching;
        char* name = give_token();
        char* start_cond = give_token();
        if (start_cond[0] != '(')
        {
            printf("Bad condition of branching");
            abort();
        }
        branch_func->data->branch_name = put_info(name);
        branch_func->right_daughter = GetCondition(array_of_var, array_of_func, mytree);
        branch_func->right_daughter->parent = branch_func;

        char* end_cond = give_token();
        if (end_cond[0] != ')')
        {
            printf("Bad condition of branching");
            abort();
        }
        free(name);
        free(start_cond);
        free(end_cond);
        free(maybe_body);
        free(end);

        return branch_func;
    }

    pos = old_pos;
    free(maybe_body);

    return nullptr;
}

vertex* GetCondition(var* array_of_var, func* array_of_func, Tree* mytree)
{
    vertex* condition = create_vertex(mytree, nullptr, nullptr);

    condition->left_son = GetExpression(array_of_var, array_of_func, mytree);
    if (condition->left_son == nullptr)
    {
        printf("Bad condition");
        abort();
    }
    condition->left_son->parent = condition;

    char* ask = give_token();

    if (ask[0] == '<' || ask[0] == '>' || ask[0] == '=')
    {
        condition->data->oper_name = put_info(ask);
    }
    else
    {
        printf("Bad sign in condition");
        abort();
    }
    free(ask);

    condition->right_daughter = GetExpression(array_of_var, array_of_func, mytree);
    if (condition->right_daughter == nullptr)
    {
        printf("Bad condition");
        abort();
    }
    condition->right_daughter->parent = condition;

    condition->data->type = Condition;

    return condition;
}

vertex* GetExpression (var* array_of_var, func* array_of_func, Tree* mytree)
{
    char* start = pos;

    vertex* new_child = GetM(array_of_var, array_of_func, mytree);
    vertex* new_node = nullptr;
    if(new_child == nullptr)
        return nullptr;

    char* old_pos = pos;
    char* sign = give_token();

    if (sign[0] == '+' || sign[0] == '-')
    {
        new_node = create_vertex(mytree, nullptr, nullptr);
        assert(new_node != nullptr);
        new_node->left_son = new_child;
        new_child->parent  = new_node;

        new_node->data->type = Operator;
        if (sign[0] == '+')
            new_node->data->oper_name = put_info("+");
        else
            new_node->data->oper_name = put_info("-");
        //pos++;

        start = pos;
        new_node->right_daughter = GetM(array_of_var, array_of_func, mytree);
        assert(new_node->right_daughter != nullptr);
        new_node->right_daughter->parent = new_node;

        CHECK_LEN(start);
    }
    else
    {
        pos = old_pos;
        free(sign);
        return new_child;
    }

    old_pos = pos;
    char* new_sign = give_token();
    if ((new_sign[0] == '+' || new_sign[0] == '-')) //&& *(pos - 1) != '+' && *(pos - 1) != '-')
    {
        RESTRUCT_OPER

        new_node->data->type = Operator;
        if (new_sign[0] == '+')
            new_node->data->oper_name = put_info("+");
        else
            new_node->data->oper_name = put_info("-");
        //pos++;

        start = pos;
        new_node->right_daughter = GetExpression(array_of_var, array_of_func, mytree);
        assert(new_node->right_daughter != nullptr);
        new_node->right_daughter->parent = new_node;

        CHECK_LEN(start);
    }
    else
    {
        pos = old_pos;
        free(new_sign);
    }

    return new_node;
}

vertex* GetM(var* array_of_var, func* array_of_func, Tree* mytree)
{
    char* start = pos;

    vertex* new_child = GetP(array_of_var, array_of_func, mytree);
    vertex* new_node = nullptr;
    if (new_child == nullptr)
        return nullptr;

    char* old_pos = pos;
    char* sign = give_token();

    if (sign[0] == '*' || sign[0] == '/')
    {
        new_node = create_vertex(mytree, nullptr, nullptr);
        assert(new_node != nullptr);
        new_node->left_son = new_child;
        new_child->parent  = new_node;

        new_node->data->type = Operator;
        if (sign[0] == '*')
            new_node->data->oper_name = put_info("*");
        else
            new_node->data->oper_name = put_info("/");
        //pos++;

        start = pos;
        new_node->right_daughter = GetP(array_of_var, array_of_func, mytree);
        assert(new_node->right_daughter != nullptr);
        new_node->right_daughter->parent = new_node;

        CHECK_LEN(start);
    }
    else
    {
        pos = old_pos;
        free(sign);
        return new_child;
    }

    old_pos = pos;
    char* new_sign = give_token();
    if ((new_sign[0] == '*' || new_sign[0] == '/')) //&& *(pos - 1) != '+' && *(pos - 1) != '-')
    {
        RESTRUCT_OPER

        new_node->data->type = Operator;
        if (new_sign[0] == '*')
            new_node->data->oper_name = put_info("*");
        else
            new_node->data->oper_name = put_info("/");
        //pos++;

        start = pos;
        new_node->right_daughter = GetM(array_of_var, array_of_func, mytree);
        assert(new_node->right_daughter != nullptr);
        new_node->right_daughter->parent = new_node;

        CHECK_LEN(start);
    }
    else
    {
        pos = old_pos;
        free(new_sign);
    }

    return new_node;
}

vertex* GetP(var* array_of_var, func* array_of_func, Tree* mytree)
{
    assert(mytree != nullptr);

    char* old_pos = pos;
    vertex* new_node = nullptr;

    char* current = give_token();

    if (current[0] == '(')
    {
        new_node = GetExpression( array_of_var, array_of_func, mytree);
        assert(new_node != nullptr);

        char* end = give_token();
        sassert(end[0] == ')');
        free(end);

        return new_node;
    }

    if ( current[0] <= '9' && current[0] >= '0')
    {
        pos = old_pos;
        new_node = GetN(mytree);
        assert(new_node != nullptr);

        return new_node;
    }
    else if (isalpha(current[0]))
    {
        char* name = current;
        assert(name != nullptr);

        char* pos_before_next = pos;
        char* next = give_token();

        if (next[0] != '(')
        {
            pos = old_pos;
            new_node = GetVar(array_of_var, array_of_func, mytree);
            if (new_node == nullptr)
            {
                printf("I don't know this variable");
                abort();
            }
            return new_node;
        }
        if (is_stand_func(name))
        {
            new_node = create_vertex(mytree, nullptr, nullptr);
            new_node->data->type = Standart_func;
            new_node->data->stand_func_name = put_info(name);
        }
        else if (is_func(name, array_of_func))
        {
            new_node = create_vertex(mytree, nullptr, nullptr);
            new_node->data->type = Function;
            new_node->data->func_name = put_info(name);
        }
        else
        {
            printf("Syntax assert!!! I don,t find this function %s ", name);
            abort();
        }

        //pos++;
        //start = pos;

        vertex* new_child = GetParameter(array_of_var, array_of_func, mytree);
        //assert(new_child != nullptr);

        char* end_of_func = give_token();
        sassert(end_of_func[0] == ')');

        new_node->right_daughter = new_child;
        new_child->parent = new_node;

        free(end_of_func);
        free(next);
        free(current);
        //free(old_pos);

        return new_node;
    }
    pos = old_pos;
    free(current);
    return new_node;
}

vertex* GetParameter(var* array_of_var, func* array_of_func, Tree* mytree)
{
    assert(mytree != nullptr);
    assert(array_of_var != nullptr);
    assert(array_of_func != nullptr);

    vertex* node = GetExpression(array_of_var, array_of_func, mytree);
    if (node == nullptr)
        return nullptr;

    vertex* param = create_vertex(mytree, nullptr, nullptr);
    param->left_son = node;
    node->parent = param;
    param->data->type = Parameter;
    param->data->oper_name = put_info("Comma");

    char* old_pos = pos;
    char* oper = give_token();

    if(strcmp(oper, ","))
    {
        free(oper);
        pos = old_pos;
        return param;
    }

    param->right_daughter = GetParameter(array_of_var, array_of_func, mytree);
    if (param->right_daughter == nullptr)
    {
        printf("I want param after - , !!!!");
        abort();
    }
    param->right_daughter->parent = param;
    free(oper);

    return param;
}

vertex* GetN(Tree* mytree)
{
    assert(mytree != nullptr);

    vertex* new_node = create_vertex(mytree, nullptr, nullptr);
    new_node->data->type = Number;

    int ans = 0;

    /*while ('0' <= *pos && *pos <= '9')
    {
        ans = ans * 10 + (*pos - '0');
        pos++;
    }

    new_node->data->number = ans;*/

    char* number_for_atoi = give_token();
    ans = atoi(number_for_atoi);
    new_node->data->number = ans;
    free(number_for_atoi);
    return new_node;
}

char* give_name()
{
    assert(pos != nullptr);

    char buff[101];
    int i = 0;
    while(*pos != '(' && *pos != ' ' && (isalpha(*pos) || isdigit(*pos)) && i < 100)/////////////// change in language
    {
        buff[i] = *pos;
        pos++;
        i++;
    }
    buff[i] = '\0';
    char* ans = put_info(buff);

    return ans;
}
bool is_func(char* string, func* array_of_func)
{
    assert(string != nullptr);

    for(int i = 0; i < num_of_func; i++)
    {
        if (array_of_func[i].name_of_func != nullptr)
        {
            if (!strcmp(string, array_of_func[i].name_of_func))
                return 1;
        }
        else
            break;
    }

    return 0;
}

bool is_stand_func(char* string)
{
    assert(string != nullptr);

    // Autogeneration code, don't touch them!!!! If you want to change - go to Functions.h and change in them(and read man)

#define DEF_SFL( name, differenciation) if(!strcmp(#name, string)) return true;

#include "Standart_func_lib.h"

#undef DEF_SFL

    return false;
}

vertex* GetVar(var* array_of_var, func* array_of_func, Tree* mytree)
{
    char* old_pos = pos;

    char* name = give_token();

    int i = 0;
    for(; i < num_of_var; i++)
    {
        if (array_of_var[i].name_of_var == nullptr)
            break;
        if (!strcmp(name, array_of_var[i].name_of_var))
        {
            vertex* node = create_vertex(mytree, nullptr, nullptr);
            node->data->type = Variable;
            node->data->var_name = put_info(name);

            free(name);
            return node;
        }
    }

    free(name);
    pos = old_pos;
    return nullptr;
}



vertex* GetRav(var* array_of_var, func* array_of_func, Tree* mytree)
{
    vertex* node = GetExpression(array_of_var, array_of_func, mytree);

    if (node == nullptr)
        return nullptr;

    char* old_pos = pos;
    char* oper = give_token();
    if (!strcmp(oper, "Solo"))
    {
        vertex* new_node = create_vertex(mytree, nullptr, nullptr);
        new_node->left_son = GetVar(array_of_var, array_of_func, mytree);
        if (new_node->left_son == nullptr)
        {
            printf("bad equality");
            abort();
        }
        new_node->left_son->parent = new_node;
        new_node->data->type = Equality;
        new_node->right_daughter = node;
        new_node->right_daughter->parent = new_node;

        return new_node;
    } else if (!strcmp("Money", oper))
    {
        vertex* new_node = create_vertex(mytree, nullptr, nullptr);
        new_node->data->type = Return;
        new_node->data->oper_name = put_info("Return");
        new_node->left_son = node;
        node->parent = new_node;

        return new_node;
    }

    free(oper);
    pos = old_pos;
    return node;
}

char* give_token()
{
    char* buff = (char*) calloc(101, sizeof(*buff));
    assert(buff != nullptr);

    char* old_pos = pos;

    int i = 0;

    while((*pos == ' ' || *pos == '\t') && *pos != '\0')
        pos++;

    for(; i < 100; i++)
    {
        if (*pos == '\0' || *pos == ' ')
            break;
        buff[i] = *pos;
        pos++;
    }
    buff[i] = '\0';

    if (pos == old_pos)
    {
        printf("Zero token!!");
        abort();
    }
    return buff;
}

//----------------------------------------------------------------------------------------------------------------------
//! Function that create buffer and fill them from reading file
//!
//! @param[in] int inFile - address of reading file
//!
//! @return - address of this buffer
//----------------------------------------------------------------------------------------------------------------------

char* fillBuff(int inFile)
{
    assert(inFile > 0);

    long int numSymbols = calcSize(way_to_in);
    assert(numSymbols != 0);

    char* buff = (char*) calloc(numSymbols + 20, sizeof(*buff));
    assert(buff != nullptr);

    long int error = read(inFile, buff, numSymbols);
    assert(error == numSymbols);

    return buff;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of symbols in file
//!
//! @param[in] const char* inFile - full name of file
//!
//! @return - number of symbols in reading file
//!
//! @Note - this function return position of reading symbol to start of file
//----------------------------------------------------------------------------------------------------------------------

long int calcSize(const char* inFile)
{
    assert(inFile != nullptr);

    struct _stat fileInfo;
    _stat(inFile, &fileInfo);

    return fileInfo.st_size;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of special symbols in array
//!
//! @param[in] char* buff- address of array
//! @param[in] char symbol - symbol that we need find
//!
//! @return - number of strings
//!
//! @Note - this function return position of reading symbol to start of file
//----------------------------------------------------------------------------------------------------------------------

size_t calc_number_of_special_symbol(char* buff, char symbol)
{
    assert(buff != nullptr);

    size_t numSymbols = 1;
    for(char* pos = strchr(buff, symbol);pos;pos = strchr(pos + 1, symbol))
        numSymbols++;

    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function create array of the strings and fill it
//!
//! @param[in] char* buff - buffer with strings that we divide
//! @param[in] size_t numStrings - number of the strings in buffer
//!
//! @return - new struct with addresses of strings and their length
//!
//! @Note this array like matrix
//----------------------------------------------------------------------------------------------------------------------

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
            addressesStrings[pos_in_addressesStrings].String = buff + i + 2;
            addressesStrings[pos_in_addressesStrings - 1].len = buff + i + 1 - first_pos;
            first_pos = addressesStrings[pos_in_addressesStrings].String;
            buff[i] = '\0';
            cut_comment(addressesStrings[pos_in_addressesStrings - 1].String);
            pos_in_addressesStrings++;
        }
    }
    cut_comment(addressesStrings[pos_in_addressesStrings - 1].String);
    addressesStrings[numStrings].String = nullptr;

    return addressesStrings;
}

//! Function for cutting comments(find // and change they to symbol of end of string)
//!
//! \param string your string
//! \return 0 if it cut comment and -1 if it didn't find comment

int cut_comment(char* string)
{
    assert(string != nullptr);

    char* new_end = strchr(string, '/');
    if (new_end != nullptr)
    {
        if (*(new_end + 1) == '/')
            *new_end = '\0';
    }
    else
        return -1;

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
//! This function calculate number of no-zero addresses
//!
//! @param[in] mystring* addressesStrings - array of strings
//!
//! @return number of no-zero addresses
//!
//! @Note WARNING!!!!!!!!!! addressesStrings must have nullptr in the end
//----------------------------------------------------------------------------------------------------------------------

size_t calc_no_zero_Addresses(mystring* addressesStrings)
{
    assert(addressesStrings != nullptr);

    size_t size = 1;
    for(; addressesStrings[size].String != nullptr; size++);
    return size;
}

//----------------------------------------------------------------------------------------------------------------------
//! This function the third part for working with arrays of strings. It destroys all memory that was allocated
//!
//! @param[in] char* buff - first param that need to destroy
//! @param[out] mystring* addressesStrings - second param that need to destroy
//!
//! @Note we destroys buffer earlier than addresses of strings, because it's a part of addresses of strings
//----------------------------------------------------------------------------------------------------------------------

void destroyer(char* buff, mystring* addressesStrings)
{
    free(buff);
    free(addressesStrings);
}

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
        delete_data(&(Judah->data));/////////////

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

vertex* create_vertex(Tree* myTree, vertex* left_vertex, vertex* right_vertex)
{
    //ASSERT(Tree_ok(myTree));

    vertex* new_vertex = create_empty_elem(myTree);
    if (new_vertex == nullptr)
        return nullptr;

    new_vertex->right_daughter = right_vertex;
    new_vertex->left_son       = left_vertex;
    new_vertex->data           = (data_for_rec_tree*) calloc(1, sizeof(*(new_vertex->data)));


    if(left_vertex != nullptr && right_vertex != nullptr)
    {
        left_vertex->parent  = new_vertex;
        right_vertex->parent = new_vertex;
    }

    // ASSERT(Tree_ok(myTree));

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

data_t* put_info(char* info)
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
    //ASSERT(Tree_ok(myTree));

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

int delete_data(data_for_rec_tree** old_data)
{
    if (old_data == nullptr)
        return -1;
    switch ((*old_data)->type) {
        case Operator: {
            if ((*old_data)->oper_name == nullptr)
                return -2;
            free((*old_data)->oper_name);
            (*old_data)->oper_name = nullptr;
            break;
        }
        case Function: {
            if ((*old_data)->func_name == nullptr)
                return -2;
            free((*old_data)->func_name);
            (*old_data)->func_name = nullptr;
            break;
        }
        case Variable: {
            if ((*old_data)->var_name == nullptr)
                return -2;
            free((*old_data)->var_name);
            (*old_data)->var_name = nullptr;
            break;
        }
        case Number: {
            (*old_data)->number = 0;
            break;
        }
        case Standart_func: {
            if ((*old_data)->stand_func_name == nullptr)
                return -2;
            free((*old_data)->stand_func_name);
            (*old_data)->stand_func_name = nullptr;
            break;
        }
        case Branching: {
            if ((*old_data)->branch_name == nullptr)
                return -2;
            free((*old_data)->branch_name);
            (*old_data)->branch_name = nullptr;
            break;
        }
        case Inicialise: {
            break;
        }
        case Equality: {
            break;
        }
        case Body: {
            break;
        }
        case Condition: {
            if ((*old_data)->oper_name == nullptr)
                return -2;
            free((*old_data)->oper_name);
            (*old_data)->oper_name = nullptr;
            break;
        }
        default: {
            printf("Error of deleting data");
            return -42;
        }
    }
    *old_data = nullptr;
    return 0;
}

void tree_dot(Tree* nameTree, char* dot_out) {
    assert(dot_out);
    ASSERT(Tree_ok(nameTree));

    FILE *file_dot = fopen(dot_out, "wb");
    assert(file_dot);

    fprintf(file_dot, "digraph {\r\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\r\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\r\n");

    create_tree(nameTree->root, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

    //system("dot -Tjpg -o tree.jpg dot_out.dot");
    //system("xviewer tree.jpg");

    ASSERT(Tree_ok(nameTree));
}

int tmp = 0;
void create_tree(vertex* pos, FILE* dot_out) {
    assert(dot_out);
    assert(pos);

    fprintf(dot_out, "\t\they%d", tmp);
    print_mode(pos, dot_out);
    fprintf(dot_out, ";\r\n");

    int iskan = tmp;

    if (pos->left_son != nullptr) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\r\n", ++tmp);

        create_tree(pos->left_son, dot_out);
    }

    if (pos->right_daughter != nullptr) {
        fprintf(dot_out, "\t\they%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "hey%d;\r\n", ++tmp);

        create_tree(pos->right_daughter, dot_out);
    }

    assert(pos);
    assert(dot_out);
}

void print_mode(vertex* pos, FILE* dot_out) {
    assert(pos);
    assert(dot_out);

    switch (pos->data->type) {
        case Function:     fprintf(dot_out, "[label = \"%s\", shape = \"Mcircle\", "
                                            "color=\"#006400\", style=\"filled\", "
                                            "fillcolor = \"#32CD32\"]", pos->data->func_name);
            break;

        case Operator:     fprintf(dot_out, "[label = \"%s\", shape = \"diamond\", "
                                            "color=\"#FFD700\", style=\"filled\", "
                                            "fillcolor = \"#F0E68C\"]", pos->data->oper_name);
            break;

        case Variable:     fprintf(dot_out, "[label = \"%s\", shape = \"component\", "
                                            "color=\"#8B008B\", style=\"filled\", "
                                            "fillcolor = \"#9932CC\"]", pos->data->var_name);
            break;

        case Number:       fprintf(dot_out, "[label = \"%d\", shape = \"Msquare\", "
                                            "color=\"#8B0000\", style=\"filled\", "
                                            "fillcolor = \"#FFA07A\"]", pos->data->number);
            break;

        case Standart_func: fprintf(dot_out, "[label = \"%s\", shape = \"doubleoctagon\", "
                                             "color=\"#006400\", style=\"filled\", "
                                             "fillcolor = \"#32CD32\"]", pos->data->stand_func_name);
            break;

        case Branching:     fprintf(dot_out, "[label = \"%s\", shape = \"box\", "
                             "color=\"#8B0000\", style=\"filled\", "
                             "fillcolor = \"#FFA07A\"]", pos->data->branch_name);
            break;

        case Inicialise: fprintf(dot_out, "[label = \"%s\", shape = \"Mdiamond\", "
                                          "color=\"#8B0000\", style=\"filled\", "
                                          "fillcolor = \"#FFA07A\"]", "Inic");
            break;

        case Equality: fprintf(dot_out, "[label = \"%s\", shape = \"invtrapezium\", "
                                        "color=\"#8B0000\", style=\"filled\", "
                                        "fillcolor = \"#FFA07A\"]", "Solo");
            break;

        case Body: fprintf(dot_out, "[label = \"%s\", shape = \"trapezium\", "
                                    "color=\"#8B0000\", style=\"filled\", "
                                    "fillcolor = \"#FFA07A\"]", "Body");
            break;

        case Condition: fprintf(dot_out, "[label = \"%s\", shape = \"septagon\", "
                                         "color=\"#8B0000\", style=\"filled\", "
                                         "fillcolor = \"#FFA07A\"]", pos->data->oper_name);
            break;

        case Return: fprintf(dot_out, "[label = \"%s\", shape = \"diamond\", "
                                      "color=\"#8B0000\", style=\"filled\", "
                                      "fillcolor = \"#FFC0CB\"]", pos->data->oper_name);
            break;

        case Parameter: fprintf(dot_out, "[label = \"%s\", shape = \"invtrapezium\", "
                                      "color=\"#8B0000\", style=\"filled\", "
                                      "fillcolor = \"#000080\"]", pos->data->oper_name);
            break;

        default:          fprintf(dot_out, "\"error\"");
            break;
    }

    assert(pos);
    assert(dot_out);
}

