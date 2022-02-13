#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

#define MAX_FUNCTION_ARGS 100

void del_atom_array(char **args, int i);
struct LispVariable *def_lambda(struct Scope *scope, char *name, struct SExpression *sargs, struct SExpression *body);
struct LispVariable *eval_named_proc(struct Scope *scope, struct LispVariable *lvar, struct SExpression *sexp);

struct LispVariable *lookupDefinition(struct Scope *scope, char *name)
{
    if (scope == NULL)
        return NULL;
    else if (scope->inner_defs == NULL)
        return lookupDefinition(scope->outer_defs, name);

    struct Definition *def;
    for (int i = 0; scope->inner_defs[i] != NULL; i++)
    {
        def = scope->inner_defs[i];
        if (strcmp(name, def->name) == 0)
        {
            return def->lvar;
        }
    }
    return lookupDefinition(scope->outer_defs, name);
}

struct LispVariable *define(struct Definition **top_level_defs, char *name, struct SExpression *sexp)
{
    for (int i = 0; i < MAX_LEVEL_DEFINES; i++)
    {
        if ((top_level_defs[i] == NULL) 
                || (strcmp(name, top_level_defs[i]->name) == 0))
        {
            struct LispVariable *lvar = eval_top_level(top_level_defs, sexp);
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
    struct Scope *scope = malloc(sizeof(*scope));
    scope->inner_defs = top_level_defs;
    scope->outer_defs = NULL;
    return eval(scope, sexp);
}


struct LispVariable *eval_procedure(struct Scope *scope, char *name, struct SExpression *sexp)
{
    struct LispVariable *lvar;
    if (strcmp(name, "+") == 0)
    {
        int sum = 0;
        struct LispVariable *lvar = addition(scope, sexp, &sum);
        if (lvar == NULL)
        {
            union LispValue *lv = malloc(sizeof(lv));
            lv->num = sum;
            lvar = newLispVariable(lv, NUM);
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
        if ((sexp != NULL))
        {
            if ((sexp->type == SEXPRESSION) && (sexp->cdr != NULL) &&
                (sexp->cdr->type == SEXPRESSION) && (sexp->cdr->car.sexp != NULL))
            {
                return def_lambda(scope, name, sexp->car.sexp, sexp->cdr);
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
    else if ((lvar = lookupDefinition(scope, name)) && lvar->lt == PROCEDURE)
    {
        return eval_named_proc(scope, lvar, sexp);
    }
    else 
    {
        printf("Error: procedure '%s' not defined\n", name);
        return NULL;
    }
}

struct LispVariable *eval_named_proc(struct Scope *scope, struct LispVariable *lvar, struct SExpression *sexp)
{
    struct Definition **defs = calloc(MAX_LEVEL_DEFINES, sizeof(**defs));
    struct SExpression *sarg = sexp;
    char **args = lvar->lv.proc->args;
    int i;
    for (i = 0; i < MAX_FUNCTION_ARGS && args[i] != NULL && sarg != NULL; i++, sarg = sarg->cdr)
    {
        defs[i] = malloc(sizeof(struct Definition));
        defs[i]->name = calloc(BUFF_SIZE, sizeof(defs[i]->name));
        strcpy(defs[i]->name, args[i]);
        if (sarg->type == SEXPRESSION || sarg->type == ATOM)
        {
            defs[i]->lvar = eval(scope, sarg);
        }
        else
        {
            defs[i]->lvar = newLispVariable(&sarg->car, sarg->type);
        }
    }
    if (i == MAX_FUNCTION_ARGS - 1 && args[i] != NULL)
    {
        printf("Error: maximum number of arguments for function exceeded\n");
        return NULL;
    }
    else if ((args[i] == NULL) && (sarg != NULL))
    {
        printf("Error: too many arguments given\n");
        return NULL;
    }
    else if ((args[i] != NULL) && (sarg == NULL))
    {
        printf("Error: not enough arguments given\n");
        return NULL;
    }
    struct Scope *new_scope = malloc(sizeof(*new_scope));
    new_scope->inner_defs = defs;
    new_scope->outer_defs = scope;
    return eval_procedure(new_scope, lvar->lv.proc->proc->car.atom, lvar->lv.proc->proc->cdr);
}

struct LispVariable *def_lambda(struct Scope *scope, char *name, struct SExpression *sargs, struct SExpression *body)
{
    int i;
    struct SExpression *arg;
    char **args = calloc(MAX_FUNCTION_ARGS, sizeof(**args));
    for (i = 0, arg = sargs; i < MAX_FUNCTION_ARGS && arg != NULL && arg->type == ATOM; i++, arg = arg->cdr)
    {
        args[i] = calloc(BUFF_SIZE, sizeof(*(args[i])));
        strcpy(args[i], arg->car.atom);
    }
    if (i == MAX_FUNCTION_ARGS - 1)
    {
        printf("Error: function has more arguments than max of '%d'\n", MAX_FUNCTION_ARGS);
        del_atom_array(args, i);
        return NULL;
    }
    else if (sargs->type != ATOM)
    {
        printf("Error: Non-symbol argument given as parameter for lambda\n");
        del_atom_array(args, i);
        return NULL;
    }
    struct LispProcedure *lproc = malloc(sizeof(*lproc));
    lproc->args = args;
    lproc->proc = body->car.sexp;
    union LispValue *lv = malloc(sizeof(*lv));
    lv->proc = lproc;
    return newLispVariable(lv, PROCEDURE);
}

void del_atom_array(char **args, int i)
{
    for (;i != 0; i--) 
        free(args[i]);
    free(args);
}

struct LispVariable *eval(struct Scope *scope, struct SExpression *sexp)
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
                        return eval_procedure(scope, subexp->car.atom, subexp->cdr);
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
                        return eval_procedure(scope, subexp->car.atom, subexp->cdr);
                    }
                }
                else 
                {
                    printf("you are dumb\n");
                }
                break;
            }
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
                struct LispVariable *lvar = lookupDefinition(scope, sexp->car.atom);
                return lvar;
            }
        case NIL:
            break;
    }
    return NULL;
}

// If success, returns null. Otherwise it returns the bad lisp value
struct LispVariable *addition(struct Scope *scope, struct SExpression *sexp, int *init_val)
{
    if (sexp == NULL)
        return NULL;

    if (sexp->type == NUM)
    {
        *init_val = *init_val + sexp->car.num;
        return addition(scope, sexp->cdr, init_val);
    }
    else if ((sexp->type == SEXPRESSION) || (sexp->type == ATOM))
    {
        struct LispVariable *lvar = eval(scope, sexp);
        if ((lvar != NULL) && (lvar->lt == NUM))
        {
            *init_val = *init_val + lvar->lv.num;
            return addition(scope, sexp->cdr, init_val);
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

