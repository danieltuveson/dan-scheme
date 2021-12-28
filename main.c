#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFF_SIZE 1000
#define TAB_SIZE 4

void println(char *str)
{
    printf("%s\n", str);
}

union LispValue
{
    int num;
    char *string;
    char *atom;
    struct SExpression *sexp;
};

enum LispType
{
    NUM,
    STRING,
    ATOM,
    SEXPRESSION,
    NIL
};

struct SExpression 
{
    union LispValue car;
    enum LispType type;
    struct SExpression *cdr;
};

struct SExpression *newSExp(union LispValue car, enum LispType t)
{
    struct SExpression *sexp = malloc(sizeof(struct SExpression));
    if (sexp == NULL)
        return NULL;

    // TODO handle non-atom case
    switch (t)
    {
        // TODO handle non-atom cases
        case ATOM:
            sexp->car.atom = malloc(sizeof(char) * strlen(car.atom));
            if (sexp->car.atom == NULL)
            {
                free(sexp);
                return NULL;
            }
            else
            {
                strcpy(sexp->car.atom, car.atom);
                sexp->type = ATOM;
            }
            break;
        case NUM:
            break;
        case STRING:
            break;
        case SEXPRESSION:
            // TODO
            sexp->car.sexp = car.sexp;
            sexp->type = SEXPRESSION;
            break;
        case NIL:
            break;
    }

    return sexp;
}

// TODO make this not inefficient
struct SExpression *append(struct SExpression *sexp, union LispValue lv, enum LispType t)
{
    if (sexp == NULL)
        return NULL;
    struct SExpression *sexp_head = sexp;
    while(sexp->cdr != NULL)
        sexp = sexp->cdr;
    sexp->cdr = newSExp(lv, t);
    return sexp_head;
}

struct SExpression *concat(struct SExpression *sexp0, struct SExpression *sexp1)
{
    if (sexp0 == NULL)
        return NULL;
    struct SExpression *sexp_head = sexp0;
    while(sexp0->cdr != NULL)
        sexp0 = sexp0->cdr;
    sexp0->cdr = sexp1;
    return sexp_head;
}

void pad(int numspaces)
{
    for (int i = 0; i < numspaces; i++)
        putchar(' ');
}

void print_sexp(struct SExpression *sexp, int numspaces)
{
    int indent = TAB_SIZE + numspaces;
    pad(numspaces);
    println("{");
    while (sexp != NULL)
    {
        println("okay come on");
        printf("'%p'\n", sexp);
        switch (sexp->type)
        {
            // TODO handle non-atom cases
            case ATOM:
                pad(indent);
                printf("content: '%s'\n", sexp->car.atom);
                pad(indent);
                printf("type: atom\n");
                break;
            case NUM:
                break;
            case STRING:
                break;
            case SEXPRESSION:
                print_sexp(sexp->car.sexp, indent);
                println("maybe this happens?");
                break;
            case NIL:
                break;
        }
        sexp = sexp->cdr;
        println("maybe this doesn't??");
    }
    pad(numspaces);
    println("}");
}


void delete_sexpression(struct SExpression *sexp)
{
    if (sexp == NULL)
        return;
    delete_sexpression(sexp->cdr);
    switch (sexp->type)
    {
        // TODO handle non-atom cases
        case ATOM:
            free(sexp->car.atom);
            free(sexp);
            break;
        case NUM:
            break;
        case STRING:
            break;
        case SEXPRESSION:
            break;
        case NIL:
            break;
    }
}

void test_stuff()
{
    println("... runnning tests ...");
    union LispValue c0, c1, c2;
    struct SExpression *s0, *s1;

    // testing new
    println("... testing new ...");
    c0.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c0.atom, "this-is-an-atom");
    s0 = newSExp(c0, ATOM);
    assert(strcmp(s0->car.atom, "this-is-an-atom") == 0);
    assert(s0->cdr == NULL);

    // testing concat
    println("... testing concat ...");
    c1.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c1.atom, "this-is-another-string");
    s1 = newSExp(c1, ATOM);
    concat(s1, s0);
    assert(strcmp(s1->cdr->car.atom, "this-is-an-atom") == 0);
    assert(strcmp(s1->car.atom, "this-is-another-string") == 0);
    assert(s1->cdr->cdr == NULL);

    // testing change to sexpression element
    strcpy(s0->car.atom, "s0-is-changed");
    assert(strcmp(s1->cdr->car.atom, "s0-is-changed") == 0);

    // testing append
    println("... testing append ...");
    c2.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c2.atom, "another-atom");
    append(s1, c2, ATOM);
    assert(strcmp(s1->car.atom, "this-is-another-string") == 0);
    assert(strcmp(s1->cdr->car.atom, "s0-is-changed") == 0);
    assert(strcmp(s1->cdr->cdr->car.atom, "another-atom") == 0);
    assert(s1->cdr->cdr->cdr == NULL);

    println("... tests finished ...");
}


// TODO deal with leading whitespace before first element
struct SExpression *parse_sexpression(char buffer[BUFF_SIZE], int *start_index)
{
    char current_word[BUFF_SIZE];
    int i, c, prevc, start_token;
    c = buffer[*start_index];
    if (c != '(') 
    {
        println("Parse error: sexpression must begin with '('");
        return NULL;
    }

    struct SExpression *sexp;
    union LispValue lv;
    prevc = c;
    start_token = *start_index + 1;
    memset(current_word, '\0', BUFF_SIZE);

    for (i = *start_index + 1; i < BUFF_SIZE; i++)
    {
        c = buffer[i];
        printf("start index: %d, ", *start_index);
        printf("i: %d, ", i);
        printf("c: '%c', ", c);
        printf("prevc: '%c'", prevc);
        printf("current word: '%s'\n", current_word);
        if (c == '(')
        {
            int old_start = *start_index;
            *start_index = i;
            struct SExpression *sub_sexp = parse_sexpression(buffer, start_index);
            i = *start_index;
            *start_index = old_start;
            lv.sexp = sub_sexp;
            if (start_token == (*start_index + 1))
                sexp = newSExp(lv, SEXPRESSION);
            else 
                append(sexp, lv, SEXPRESSION);
            // treat s-expression like a space character
            c = ' ';
        }
        if (c == '\0' || c == ')') 
        {
            break;
        }
        else if (prevc != ' ' && c == ' ')
        {
            lv.atom = malloc(sizeof(char) * BUFF_SIZE);
            strcpy(lv.atom, current_word);
            if (start_token == (*start_index + 1))
                sexp = newSExp(lv, ATOM);
            else 
            {
                append(sexp, lv, ATOM);
            }
            free(lv.atom);
            memset(current_word, '\0', BUFF_SIZE);
        }
        else if (prevc == ' ' && c != ' ')
        {
            start_token = i;
            current_word[i-start_token] = buffer[i];
        }
        else if (prevc != ' ' && c != ' ')
        {
            current_word[i-start_token] = buffer[i];
        }
        prevc = c;
        print_sexp(sexp, 0);
    }
    if (c != ')') 
    {
        println("Parse error: sexpression must end with ')'");
        delete_sexpression(sexp);
        return NULL;
    }
    if (current_word[0] != '\0') 
    {
        lv.atom = malloc(sizeof(char) * BUFF_SIZE);
        strcpy(lv.atom, current_word);
        if (start_token == (*start_index + 1))
            sexp = newSExp(lv, ATOM);
        else 
        {
            println("this is where it happsn");
            append(sexp, lv, ATOM);
        }
        free(lv.atom);
    }
    *start_index = i;
    return sexp;
}

int main()
{
    test_stuff();

    while (true)
    {
        printf("> ");
        // Read input
        char new_buff[BUFF_SIZE];
        char test_command[] = "(thing (and another) thing)";
        memset(new_buff, '\0', BUFF_SIZE);
        strcpy(new_buff, test_command);
        println(new_buff);
        // fgets(new_buff, BUFF_SIZE, stdin);

        struct SExpression *sexp, *head;
        int start = 0;
        sexp = parse_sexpression(new_buff, &start);
        head = sexp;

        // print contents of sexpression from input
        if (sexp != NULL)
            print_sexp(sexp, 0);
    }
}

