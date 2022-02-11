#ifndef INTERPRETER
#define INTERPRETER
#include "sexpression.h"

#define MAX_LEVEL_DEFINES 1000

struct Definition
{
    char *name;
    struct LispVariable *lvar;
};

struct Scope
{
    struct Definition **inner_defs;
    struct Scope *outer_defs;
};

// struct Scope *start_intepreter(int init_size);
// struct Scope newScope(struct Scope *outer_defs);
// struct SExpression *eval(struct Scope *sc);
// struct Scope *define(struct Scope *sc, char *name, struct SExpression *sexp);
// int addition(struct Scope *sc, struct SExpression *sexp, int init_val);
struct LispVariable *addition(struct Scope *scope, struct SExpression *sexp, int *init_val);
struct LispVariable *eval(struct Scope *scope, struct SExpression *sexp);
struct LispVariable *lookupDefinition(struct Scope *scope, char *name);
struct LispVariable *define(struct Definition **top_level_defs, char *name, struct SExpression *sexp);
struct LispVariable *eval_top_level(struct Definition **top_level_defs, struct SExpression *sexp);
struct LispVariable *eval_procedure(struct Scope *scope, char *name, struct SExpression *sexp);
struct LispProcedure *lambda(struct SExpression *arguments, struct SExpression *procedure);

#endif
