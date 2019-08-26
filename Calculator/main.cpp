#include <cstdio>
#include <cassert>

#define sassert( x ) \
{                                                                                                                       \
    if (!(x))                                                                                                           \
    {                                                                                                                   \
        printf("Syntax error in %d line in %s", __LINE__, __PRETTY_FUNCTION__);                                         \
        if (position != nullptr)                                                                                        \
            printf(" symbol is %c", *position);                                                                         \
        abort();                                                                                                        \
    }                                                                                                                   \
}

#define CHECK_LEN( x ) \
{                                                                                                                       \
    if ((x) == position)                                                                                                \
    {                                                                                                                   \
        printf("Errorororo I began cleaning your laptop\n");                                                            \
        printf("P.s. Don't panic, it was joke(maybe)))). Length = 0 in %s in %d line\n", __PRETTY_FUNCTION__, __LINE__);\
        if (position != nullptr)                                                                                        \
            printf(" symbol is %c", *position);                                                                         \
        abort();                                                                                                        \
    }                                                                                                                   \
}

//! This is pointer to the current position in your expression (global, because all functions need access)
char* position = nullptr;


//! This function calculate your expression
//!
//! \param string pointer to your expression
//! \return answer
int calculator(char* string);

//! This function is first symbol G ::= E'\0'
//!
//! \param string your expression
//! \return answer
int GetG(char* string);

//! E ::= T{[+-]T}*
//!
//! \return answer
int GetE();

//! T ::= P{[*/]P}*
//!
//! \return answer
int GetT();

//! P :: = (E) | N
//!
//! \return answer
int GetP();

//! N ::= [0-9]+
//!
//! \return answer
int GetN();

int main() {
    printf("#Hello, you use calculator with norm parcer\n\n");
    printf("#Please, support next projects 89167221841\n\n");

    char string[1000] = "";
    gets(string);

    int answer = calculator(string);

    printf("Your answer = %d", answer);

    return 0;
}

int calculator(char* string)
{
    assert(string != nullptr);

    int ans = GetG(string);

    return ans;
}

int GetG(char* string)
{
    position = string;
    int ans = GetE();
    if (position == string)
    {
        printf("Syntax errrrrrrrrrrors, I began cleaning your laptop\n");
        printf("P.s. if you want to debug this program length = 0 in G\n");
        abort();
    }
    sassert(*position == '\0');
    return ans;
}

int GetE()
{
    char* old_position = position;
    int ans = GetT();

    CHECK_LEN(old_position);

    while(*position == '+' || *position == '-')
    {
        int symbol = *position;
        position++;
        char* pos_before = position;

        int inter_ans = GetT();
        CHECK_LEN(pos_before);

        if (symbol == '+')
            ans += inter_ans;
        else
            ans -= inter_ans;
    }

    return ans;
}

int GetT()
{
    char* old_position = position;
    int ans = GetP();

    CHECK_LEN(old_position);

    while(*position == '*' || *position == '/')
    {
        int symbol = *position;
        position++;

        char* pos_before = position;
        int inter_ans = GetP();
        CHECK_LEN(pos_before);

        if (symbol == '*')
            ans *= inter_ans;
        else
            ans /= inter_ans;
    }
    return ans;
}

int GetP()
{
    int ans = 0;

    if (*position == '(')
    {
        position++;

        char* old_pos = position;
        ans = GetE();

        CHECK_LEN(old_pos);
        sassert(*position == ')');

        position++;
    }
    else
    {
        char* old_pos = position;
        ans = GetN();

        CHECK_LEN(old_pos);
    }

    return ans;
}

int GetN()
{
    int ans = 0;

    while ('0' <= *position && *position <= '9')
    {
        ans = ans * 10 + (*position - '0');
        position++;
    }

    return ans;
}

