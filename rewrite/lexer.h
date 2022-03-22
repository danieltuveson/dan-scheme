#ifndef LEXER
#define LEXER

typedef enum
{
    NO_ERROR,
    UNEXPECTED_END_OF_INPUT,
    STRING_MATCH_FAILURE
} LexError;

typedef enum 
{
    LTrue,
    LFalse
} LBoolean;

typedef struct
{
    enum 
    {
        LBOOLEAN
    } Type;
    union
    {
        LBoolean lbool;
    } Value;
} LexerContent;

typedef struct 
{
    char *input;
    // LexerContent content;
    int charsLexed;
    int size;
    bool hasNext;
    int bookmark;
    LexError error; // this gets set if encounters error while lexing
} Lexer;

#endif
