#ifndef LEXER
#define LEXER

#include <stdbool.h>
#include "linked_list.h"

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

typedef char *Comment;

typedef struct
{
    Whitespace whitespace;
    Comment comment;
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
    union
    {
        char Char;
        enum
        {
            Space,
            Newline
        };
    } value;
} LCharacter;

typedef struct
{
    enum 
    {
        SPACE_OR_COMMENT,
        LBOOLEAN
    } type;
    union
    {
        LBoolean lbool;
    } value;
    LinkedList list;
} LexerContent;

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

typedef struct 
{
    char *input;
    // LexerContent content;
    unsigned long charsLexed;
    unsigned long size;
    bool hasNext;
    Bookmark *bookmark;
    LexError error; // this gets set if encounters error while lexing
} Lexer;

#include "lexer.c.generated.h"
#endif
