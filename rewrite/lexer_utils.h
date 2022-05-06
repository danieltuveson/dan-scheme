#ifndef LEXER_UTILS
#define LEXER_UTILS


static inline bool isWhitespace(char c)
{
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

static inline bool isDelimiter(char c)
{
    return (isWhitespace(c) || c == '\"' || c == '(' || c == ')' || c == ';' || c == '\0');
}

#include "lexer_utils.c.generated.h"
#endif
