#ifndef TOKENS
#define TOKENS

#define MAX_IDENTIFIER_LENGTH 100

typedef enum
{
    NO_ERROR,
    UNEXPECTED_END_OF_INPUT,
    STRING_MATCH_FAILURE,
    EXPECTING_DELIMITER
} LexError;

// typedef enum
// {
//     SPACE,
//     TAB,
//     NEWLINE
// } Whitespace;

typedef char Comment;

// typedef struct
// {
//     Whitespace whitespace;
//     Comment *comment;
// } IntertokenSpace;

typedef enum 
{
    LTrue,
    LFalse
} LBoolean;

typedef enum 
{
    LOPEN_PAREN,
    LCLOSE_PAREN,
    LQUOTE
} SimpleToken;

typedef struct
{
    enum 
    {
        // SPACE_OR_COMMENT,
        LBOOLEAN,
        LCHARACTER,
        LINT,
        LIDENTIFIER,
        LSIMPLE_TOKEN
    } type;
    union
    {
        LBoolean lbool;
        char lchar;
        int lint;
        char *lidentifier;
        SimpleToken lsimpletoken;
    } value;
} Token;

typedef struct Tokens
{
    Token *token;
    struct Tokens *next;
} Tokens;


#include "tokens.c.generated.h"
#endif
