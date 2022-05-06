#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "lexer_utils.h"

void matchString(Lexer *lexer, char *str)
{
    char c;
    setBookmark(lexer);
    for (unsigned int i = 0; i < strlen(str); i++)
    {
        if (!hasNext(lexer))
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
    if (hasNext(lexer))
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
    if (!hasNext(lexer))
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
    else if (!isDelimiter(peek(lexer)))
    {
        lexer->error = EXPECTING_DELIMITER;
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
    {
        lexer->token = newBoolToken(LFalse);
        return;
    }

    matchDelimitedString(lexer, "#t");
    if (lexer->error == NO_ERROR)
    {
        lexer->token = newBoolToken(LTrue);
        return;
    }
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

int lexSign(Lexer *lexer)
{
    char c = peek(lexer);

    if (c == '+' || c == '-')
        nextChar(lexer);

    return (c == '-' ? (-1) : 1);
}

// Parse 0 or more digits. False means no digits were parsed
bool lexDigits(Lexer *lexer)
{
    if (!isdigit(peek(lexer)))
        return false;
    char c;
    int num = 0;
    int exp = 1;
    do
    {
        c = nextChar(lexer);
        num = (exp * num) + digittoint(c);
        exp = 10;
    } while (isdigit(peek(lexer)));

    lexer->token = newIntToken(num);
    return true;
}

// Just going to implement real base 10 for now
// TODO: Implement rest of numbers
void lexNumber(Lexer *lexer)
{
    setBookmark(lexer);
    int sign = lexSign(lexer);
    if (!lexDigits(lexer))
    {
        lexer->error = STRING_MATCH_FAILURE;
        returnToBookmark(lexer);
        return;
    }
    else if (!isDelimiter(peek(lexer)))
    {
        lexer->error = EXPECTING_DELIMITER;
        returnToBookmark(lexer);
        return;
    }

    lexer->token->value.lint = sign * lexer->token->value.lint;
    return;
}

void lexSingleToken(Lexer *lexer)
{
    lexNumber(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexBoolean(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexCharacter(lexer);
    if (lexer->error == NO_ERROR)
        return;
}

void lexAllInput(Lexer *lexer)
{
    while (lexer->error == NO_ERROR && hasNext(lexer))
        lexSingleToken(lexer);
}


