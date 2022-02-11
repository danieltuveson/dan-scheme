/* File contains functions for working with S-expressions and lisp types */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "sexpression.h"

struct LispVariable *newLispVariable(union LispValue *lv, enum LispType lt)
{
    struct LispVariable *lvar = malloc(sizeof(lvar));
    lvar->lv = *lv;
    lvar->lt = lt;
    return lvar;
}

struct SExpression *newSExp(union LispValue car, enum LispType t)
{
    struct SExpression *sexp = malloc(sizeof(struct SExpression));
    if (sexp == NULL)
        return NULL;

    // TODO handle non-atom case
    switch (t)
    {
        // TODO handle non-atom cases
        case BOOLEAN:
            break;
        case SYMBOL:
            break;
        case CHAR:
            break;
        case VECTOR:
            break;
        case PROCEDURE:
            break;
        case NUM:
            sexp->car.num = car.num;
            sexp->type = NUM;
            break;
        case STRING:
            break;
        case PORT:
            break;
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
        case SEXPRESSION:
            // TODO
            sexp->car.sexp = car.sexp;
            sexp->type = SEXPRESSION;
            break;
        case NIL:
            break;
    }
    sexp->cdr = NULL;

    return sexp;
}

// TODO make this not inefficient
// Appends value to end of sexpression. 
// If sexpression is null, then creates a new one, so can be used like a constructor
struct SExpression *append(struct SExpression *sexp, union LispValue lv, enum LispType t)
{
    if (sexp == NULL)
        return newSExp(lv, t);
    struct SExpression *temp = sexp;
    while(temp->cdr != NULL)
        temp = temp->cdr;
    temp->cdr = newSExp(lv, t);
    return sexp;
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

void print_lisp_value(union LispValue lv, enum LispType lt)
{
    switch (lt)
    {
        // TODO handle non-atom cases
        case BOOLEAN:
            break;
        case SYMBOL:
            break;
        case CHAR:
            break;
        case VECTOR:
            break;
        case PROCEDURE:
            break;
        case NUM:
            printf("%d", lv.num);
            break;
        case STRING:
            break;
        case PORT:
            break;
        case ATOM:
            printf("%s", lv.atom);
            break;
        case SEXPRESSION:
            printf("(");
            print_lisp_value(lv.sexp->cdr->car, lv.sexp->cdr->type);
            printf("...)");
            break;
        case NIL:
            break;
    }
}

void print_sexp(struct SExpression *sexp, int indent)
{
    while (sexp != NULL)
    {
        switch (sexp->type)
        {
            // TODO handle non-atom cases
            case BOOLEAN:
                break;
            case SYMBOL:
                break;
            case CHAR:
                break;
            case VECTOR:
                break;
            case PROCEDURE:
                pad(indent);
                printf("type: procedure\n");
                pad(indent);
                printf("args:\n");
                for (char **ptr = sexp->car.proc->args; ptr != NULL; ptr++)
                {
                    pad(indent + TAB_SIZE);
                    printf("- %s\n", *ptr);
                }
                pad(indent);
                printf("body:\n");
                print_sexp(sexp->car.proc->proc, indent + TAB_SIZE);
                break;
            case NUM:
                pad(indent);
                printf("type: number\n");
                pad(indent);
                printf("content: '%d'\n", sexp->car.num);
                break;
            case STRING:
                break;
            case PORT:
                break;
            case ATOM:
                pad(indent);
                printf("type: atom\n");
                pad(indent);
                printf("content: '%s'\n", sexp->car.atom);
                break;
            case SEXPRESSION:
                pad(indent);
                printf("type: s-expression\n");
                pad(indent);
                printf("content:\n");
                pad(indent);
                println("{");
                print_sexp(sexp->car.sexp, TAB_SIZE + indent);
                pad(indent);
                println("}");
                break;
            case NIL:
                break;
        }
        sexp = sexp->cdr;
    }
}


void delete_sexpression(struct SExpression *sexp)
{
    if (sexp == NULL)
        return;
    delete_sexpression(sexp->cdr);
    switch (sexp->type)
    {
        // TODO handle non-atom cases
        case BOOLEAN:
            break;
        case SYMBOL:
            break;
        case CHAR:
            break;
        case VECTOR:
            break;
        case PROCEDURE:
            break;
        case NUM:
            break;
        case STRING:
            break;
        case PORT:
            break;
        case ATOM:
            free(sexp->car.atom);
            free(sexp);
            break;
        case SEXPRESSION:
            delete_sexpression(sexp->car.sexp);
            free(sexp);
            break;
        case NIL:
            break;
    }
}

