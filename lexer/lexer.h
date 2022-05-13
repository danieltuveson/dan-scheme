#ifndef LEXER
#define LEXER

#include <stdbool.h>
#include "tokens.h"

/* Most of the enums and structs in this file are used to represent lexical 
 * tokens in Scheme.
 */

#define DELIMITER " \n\t()\";"

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
    // Token *token;
    Tokens *tokens;

    // Number of characters lexer has consumed
    unsigned long charsLexed;

    // Strlen of lexer->input (should not be changed)
    unsigned long size;

    // A stack of locations in the input we may want to return to in the future
    // if we encounter an error in the middle of attempting to lex a token
    Bookmark *inputBookmark;

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

static inline void addToken(Lexer *lexer, Token *token)
{
    lexer->tokens = push(lexer->tokens, token);
}

#include "lexer.c.generated.h"
#endif
