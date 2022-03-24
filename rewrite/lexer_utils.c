#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "lexer.h"
#include "lexer_utils.h"

void matchString(Lexer *lexer, char *str)
{
    char c;
    setBookmark(lexer);
    for (unsigned int i = 0; i < strlen(str); i++)
    {
        if (!lexer->hasNext)
        { 
            lexer->error = UNEXPECTED_END_OF_INPUT;
            returnToBookmark(lexer);
            return;
        }
        c = nextChar(lexer); 
        if (c != str[i]) 
        {
            lexer->error = STRING_MATCH_FAILURE;
            returnToBookmark(lexer);
            return;
        }
    }
    lexer->error = NO_ERROR;
    unsetBookmark(lexer);
    return;
}

void matchAnyChar(Lexer *lexer)
{
    setBookmark(lexer);
    if (lexer->hasNext)
    {
        nextChar(lexer); 
        lexer->error = NO_ERROR;
        unsetBookmark(lexer);
        return;
    }
    lexer->error = UNEXPECTED_END_OF_INPUT;
    returnToBookmark(lexer);
    return;
}

bool oneOf(char c, char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
    {
        if (str[i] == c)
            return true;
    }
    return false;
}

void matchDelimiter(Lexer *lexer)
{
    // I believe end of input should be a valid delimiter
    if (!lexer->hasNext)
    {
        lexer->error = NO_ERROR;
        return;
    }

    setBookmark(lexer);
    char c = nextChar(lexer); 
    if (oneOf(c, DELIMITER))
    {
        lexer->error = NO_ERROR;
        unsetBookmark(lexer);
        return;
    }
    lexer->error = EXPECTING_DELIMITER;
    returnToBookmark(lexer);
    return;
}

void matchDelimitedString(Lexer *lexer, char *string)
{
    setBookmark(lexer);
    matchString(lexer, string);
    if (lexer->error != NO_ERROR)
    {
        returnToBookmark(lexer);
        return;
    }

    matchDelimiter(lexer);
    if (lexer->error != NO_ERROR)
    {
        returnToBookmark(lexer);
        return;
    }

    unsetBookmark(lexer);
    return;
}

void lexBoolean(Lexer *lexer)
{
    matchDelimitedString(lexer, "#f");
    if (lexer->error == NO_ERROR)
        return;
    matchDelimitedString(lexer, "#t");
}

void lexSingleCharacter(Lexer *lexer)
{
    setBookmark(lexer);
    matchString(lexer, "#\\");
    if (lexer->error != NO_ERROR)
    {
        returnToBookmark(lexer);
        return;
    }
    matchAnyChar(lexer);
    if (lexer->error != NO_ERROR)
    {
        returnToBookmark(lexer);
        return;
    }
    matchDelimiter(lexer);
    if (lexer->error != NO_ERROR)
        returnToBookmark(lexer);
    else
        unsetBookmark(lexer);
}

void lexCharacter(Lexer *lexer)
{
    // Check for "#\" prefix before trying anything else
    setBookmark(lexer);
    matchString(lexer, "#\\");
    returnToBookmark(lexer);
    if (lexer->error != NO_ERROR)
        return;

    lexSingleCharacter(lexer);
    if (lexer->error == NO_ERROR)
        return;

    matchDelimitedString(lexer, "#\\newline");
    if (lexer->error == NO_ERROR)
        return;

    matchDelimitedString(lexer, "#\\space");
    if (lexer->error == NO_ERROR)
        return;
}

