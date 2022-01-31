#ifndef PARSER
#define PARSER

#include "sexpression.h"

struct SExpression *parse_sexpression(char buffer[BUFF_SIZE], int *start_index);
struct SExpression *parse_word(struct SExpression *sexp, char *word);

#endif

