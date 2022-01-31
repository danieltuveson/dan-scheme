#ifndef SEXPRESSION_FILE
#define SEXPRESSION_FILE

#include <stdbool.h>

#define BUFF_SIZE 1000
#define TAB_SIZE 4

union LispValue
{
    bool boolean;
    char *symbol;
    char character;
    union LispValue **vect;
    int num;
    char *string;
    char *atom;
    struct SExpression *sexp;
};

enum LispType
{
    BOOLEAN,
    SYMBOL,
    CHAR,
    VECTOR,
    PROCEDURE,
    // PAIR,
    NUM,
    STRING,
    PORT,
    ATOM, // Should get rid of this in the future?
    SEXPRESSION, // Should get rid of this in the future? Actually I think this counts as a pair, maybe rename it?
    NIL
};

struct LispVariable
{
    enum LispType lt;
    union LispValue lv;
};

struct SExpression 
{
    union LispValue car;
    enum LispType type;
    struct SExpression *cdr;
};

struct LispProcedure
{
    struct SExpression *args;
    struct SExpression *proc;
};

struct LispVariable *newLispVariable(union LispValue *lv, enum LispType lt);
struct SExpression *newSExp(union LispValue car, enum LispType t);
struct SExpression *append(struct SExpression *sexp, union LispValue lv, enum LispType t);
struct SExpression *concat(struct SExpression *sexp0, struct SExpression *sexp1);
void pad(int numspaces);
void print_lisp_value(union LispValue lv, enum LispType lt);
void print_sexp(struct SExpression *sexp, int indent);
void delete_sexpression(struct SExpression *sexp);


#endif
