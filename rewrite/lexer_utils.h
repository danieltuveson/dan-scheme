#ifndef LEXER_UTILS
#define LEXER_UTILS

#include <ctype.h>

static inline bool isWhitespace(char c)
{
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

static inline bool isDelimiter(char c)
{
    return (isWhitespace(c) || c == '\"' || c == '(' || c == ')' || c == ';' || c == '\0');
}

static inline bool isAlphabet(char ch)
{
    char c = tolower(ch);
    return (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g'
            || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n'
            || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u'
            || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z');
}

static inline bool isSpecialInitial(char c)
{
    return (c == '!' || c == '$' || c == '%' || c == '&' || c == '*' || c == '/' || c == ':' 
            || c == '<' || c == '=' || c == '>' || c == '?' || c == '^' || c == '_' 
            || c == '~');
}

static inline bool isSpecialSubsequent(char c)
{
    return (c == '+' || c == '-' || c == '.' || c == '@');
}


#include "lexer_utils.c.generated.h"
#endif
