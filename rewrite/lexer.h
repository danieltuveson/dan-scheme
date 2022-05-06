#ifndef LEXER
#define LEXER

#include <stdbool.h>
#include "linked_list.h"

/* Most of the enums and structs in this file are used to represent lexical 
 * tokens in Scheme.
 */

#define DELIMITER " \n\t()\";"

typedef enum
{
    NO_ERROR,
    UNEXPECTED_END_OF_INPUT,
    STRING_MATCH_FAILURE,
    EXPECTING_DELIMITER
} LexError;

typedef enum
{
    SPACE,
    TAB,
    NEWLINE
} Whitespace;

typedef char Comment;

typedef struct
{
    Whitespace whitespace;
    Comment *comment;
} IntertokenSpace;

typedef enum 
{
    LTrue,
    LFalse
} LBoolean;

typedef struct
{
    enum
    {
        CCHAR,
        CSPACE,
        CNEWLINE
    } type;
    char Char;
} LCharacter;

typedef struct
{
    enum 
    {
        // SPACE_OR_COMMENT,
        LBOOLEAN,
        LINT,
        LCHARACTER
    } type;
    union
    {
        LBoolean lbool;
        LCharacter lcharacter;
        int lint;
    } value;
} Token;


// Linked list containing tokens
typedef struct Tokens
{
    Token *token;
    struct Tokens *prev;
    struct Tokens *next;

    // A stack of locations in the input we may want to return to in the future
    // if we encounter an error in the middle of attempting to lex a token
    // Bookmark *inputBookmark;
    unsigned long bookmark;

} Tokens;

/*  Bookmarks should be used to represent the last "good" state of our lexer
 *  If we try to lex multiple things but some fail, want to be able to return 
 *  the parser to the last character where the state of the lexer was "good"
 *
 *  E.g. if we try to parse the string "#\h", we might try a boolean. If that
 *  "#t", and that fails, "#\h" might work, so we want to return our parser 
 *  to the location of the '#' character
 */
typedef struct Bookmark
{
    unsigned long value;
    struct Bookmark *prevBookmark;
} Bookmark;


// Structure of storing the state of the lexer
typedef struct 
{
    // String input to be lexed
    char *input;

    // Content lexed so far
    Tokens *tokens;
    
    // Number of characters lexer has consumed
    unsigned long charsLexed;

    // Strlen of lexer->input (should not be changed)
    unsigned long size;

    // Stores the reason for failure when the lexer fails to successfully parse
    // a token, the reason for failure
    LexError error; // this gets set if encounters error while lexing
} Lexer;

// True if more characters left to parse in input
// False if we have reached the end of the input
static inline bool hasNext(Lexer *lexer)
{
    return (lexer->charsLexed < lexer->size);
}

static inline char peek(Lexer *lexer)
{ 
    return (!hasNext(lexer) ? '\0' : lexer->input[lexer->charsLexed]);
}

#include "lexer.c.generated.h"
#endif
