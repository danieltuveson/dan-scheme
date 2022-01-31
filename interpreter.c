#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

struct Definition **new_scope(

struct LispVariable *lookupDefinition(struct Definition **definitions, char *name)
{
    struct Definition *def;
    for (int i = 0; i < MAX_LEVEL_DEFINES; i++)
    {
        def = definitions[i];
        if (def == NULL)
        {
            break;
        }
        else if (strcmp(name, def->name) == 0)
        {
            return def->lvar;
        }

    }
    return NULL;
}

struct LispVariable *define(struct Definition **top_level_defs, char *name, struct SExpression *sexp)
{
    for (int i = 0; i < MAX_LEVEL_DEFINES; i++)
    {
        if ((top_level_defs[i] == NULL) 
                || (strcmp(name, top_level_defs[i]->name) == 0))
        {
            struct LispVariable *lvar = eval(top_level_defs, sexp);
            struct Definition *def = malloc(sizeof(*def));
            def->name = name;
            def->lvar = lvar; 
            top_level_defs[i] = def;
            return lvar;
        }
    }
    return NULL;
}

struct LispVariable *eval_top_level(struct Definition **top_level_defs, struct SExpression *sexp)
{
    if (sexp->type == SEXPRESSION)
    {
        struct SExpression *subexp = sexp->car.sexp;
        if (subexp->type == ATOM)
        {
            if (strcmp(subexp->car.atom, "define") == 0)
            {
                if ((subexp->cdr != NULL) 
                        && (subexp->cdr->type == ATOM) 
                        && (subexp->cdr->cdr != NULL))
                {
                    struct LispVariable *lvar = 
                        define(top_level_defs, subexp->cdr->car.atom, subexp->cdr->cdr);
                    if (lvar == NULL)
                        printf("Error: could not define variable\n");
                    return lvar;
                }
                else if ((subexp->cdr == NULL) || (subexp->cdr->cdr != NULL))
                {
                    printf("Error: define requires two arguments\n");
                    return NULL;
                } 
                else if (subexp->cdr->type != ATOM)
                {
                    printf("Error: first argument of define must be an atom\n");
                    return NULL;
                }
            }
        }
    }
    return eval(top_level_defs, sexp);
}

struct LispVariable *eval_procedure(struct Definition **top_level_defs, char *name, struct SExpression *sexp)
{
    if (strcmp(name, "+") == 0)
    {
        int sum = 0;
        struct LispVariable *lvar = addition(top_level_defs, sexp, &sum);
        if (lvar == NULL)
        {
            union LispValue *lv = malloc(sizeof(lv));
            lv->num = sum;
            struct LispVariable *lvar = newLispVariable(lv, NUM);
            return lvar;
        }
        else
        {
            printf("Error: non-numeric value '");
            print_lisp_value(lvar->lv, lvar->lt);
            printf("' in procedure.\n");
            return NULL;
        }
    }
    else if (strcmp(name, "lambda") == 0)
    {
        if ((sexp != NULL) )
        {
            if ((sexp->type == SEXPRESSION) && (sexp->cdr != NULL))
            {
            }
            // TODO: Define other versions of lambda argument / body
            else
            {
                printf("Error: ill-formatted lambda expression\n");
                return NULL;
            }
        }
        else
        {
            printf("Error: lambda a list of arguments and a procedure\n");
            return NULL;
        }
    }
    else 
    {
        printf("Error: procedure '%s' not defined\n", name);
        return NULL;
    }
}

struct LispVariable *eval(struct Definition **top_level_defs, struct SExpression *sexp)
{
    switch (sexp->type)
    {
        case SEXPRESSION:
            {
                struct SExpression *subexp = sexp->car.sexp;
                if (subexp->type == ATOM)
                {
                    if (strcmp(subexp->car.atom, "define") == 0)
                    {
                        printf("Error: can only define values at the top level\n");
                        return NULL;
                    }
                    else
                    {
                        return eval_procedure(top_level_defs, subexp->car.atom, subexp->cdr);
                    }
                }
                break;
            }
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
            {
                struct LispVariable *lvar = newLispVariable(&sexp->car, NUM);
                return lvar;
            }
        case STRING:
            break;
        case PORT:
            break;
        case ATOM:
            {
                struct LispVariable *lvar = lookupDefinition(top_level_defs, sexp->car.atom);
                return lvar;
            }
        case NIL:
            break;
    }
    return NULL;
}

// If success, returns null. Otherwise it returns the bad lisp value
struct LispVariable *addition(struct Definition **definitions, struct SExpression *sexp, int *init_val)
{
    if (sexp == NULL)
        return NULL;

    if (sexp->type == NUM)
    {
        *init_val = *init_val + sexp->car.num;
        return addition(definitions, sexp->cdr, init_val);
    }
    else if ((sexp->type == SEXPRESSION) || (sexp->type == ATOM))
    {
        struct LispVariable *lvar = eval(definitions, sexp);
        if ((lvar != NULL) && (lvar->lt == NUM))
        {
            *init_val = *init_val + lvar->lv.num;
            return addition(definitions, sexp->cdr, init_val);
        }
        else
        {
            printf("Error: non-numeric argument given to '+'\n");
            return NULL;
        }
    }
    else 
    {
        *init_val = 0;
        return newLispVariable(&sexp->car, sexp->type);
    }
}

struct LispProcedure *lambda(struct SExpression *arguments, struct SExpression *procedure)
{
    if (arguments->type == SEXPRESSION)
    {
        printf("all gucci\n");
    }
    else if (arguments->type == ATOM)
    {
        printf("also all gucci\n");
    }
    else 
    {
        printf("no good, hombre\n");
    }
    return NULL;
}

