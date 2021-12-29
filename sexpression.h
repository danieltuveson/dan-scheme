#ifndef SEXPRESSION_FILE
#define SEXPRESSION_FILE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"

#define BUFF_SIZE 1000
#define TAB_SIZE 4

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

struct SExpression *newSExp(union LispValue car, enum LispType t);
struct SExpression *append(struct SExpression *sexp, union LispValue lv, enum LispType t);
struct SExpression *concat(struct SExpression *sexp0, struct SExpression *sexp1);
void pad(int numspaces);
void print_sexp(struct SExpression *sexp, int numspaces);
void delete_sexpression(struct SExpression *sexp);


#endif
