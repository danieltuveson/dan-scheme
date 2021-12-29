/* File contains functions for working with S-expressions and lisp types */
#include "sexpression.h"

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

void print_sexp(struct SExpression *sexp, int numspaces)
{
    int indent = TAB_SIZE + numspaces;
    pad(numspaces);
    println("{");
    while (sexp != NULL)
    {
        // println("okay come on");
        // printf("'%p'\n", sexp);
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
                break;
            case NIL:
                break;
        }
        sexp = sexp->cdr;
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
