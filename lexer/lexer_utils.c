#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

// bool oneOf(char c, char *str)
// {
//     for (unsigned int i = 0; i < strlen(str); i++)
//     {
//         if (str[i] == c)
//             return true;
//     }
//     return false;
// }

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
    if (isDelimiter(c))
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

bool lexComment(Lexer *lexer)
{
    if (peek(lexer) != ';')
        return false;
    while (hasNext(lexer) && nextChar(lexer) != '\n')
        /* ignore comments, for now */ ;
    return true;
}

bool lexSpaces(Lexer *lexer)
{
    if (!isWhitespace(peek(lexer)))
        return false;
    while (isWhitespace(peek(lexer)))
        nextChar(lexer);
    return true;
}

void lexBoolean(Lexer *lexer)
{
    matchDelimitedString(lexer, "#f");
    if (lexer->error == NO_ERROR)
    {
        addToken(lexer, newBoolToken(LFalse));
        return;
    }

    matchDelimitedString(lexer, "#t");
    if (lexer->error == NO_ERROR)
    {
        addToken(lexer, newBoolToken(LTrue));
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
    else if (!hasNext(lexer))
    {
        lexer->error = UNEXPECTED_END_OF_INPUT;
        returnToBookmark(lexer);
        return;
    }
    char c = nextChar(lexer);

    matchDelimiter(lexer);
    if (lexer->error != NO_ERROR)
        returnToBookmark(lexer);
    else
    {
        unsetBookmark(lexer);
        addToken(lexer, newCharToken(c));
    }
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
    {
        addToken(lexer, newCharToken('\n'));
        return;
    }

    matchDelimitedString(lexer, "#\\space");
    if (lexer->error == NO_ERROR)
    {
        addToken(lexer, newCharToken(' '));
        return;
    }
}

int lexSign(Lexer *lexer)
{
    char c = peek(lexer);

    if (c == '+' || c == '-')
        nextChar(lexer);

    return (c == '-' ? (-1) : 1);
}

// Parse 0 or more digits. False means no digits were parsed
int lexDigits(Lexer *lexer)
{
    if (!isdigit(peek(lexer)))
        return -1;
    char c;
    int num = 0;
    int exp = 1;
    do
    {
        c = nextChar(lexer);
        num = (exp * num) + digittoint(c);
        exp = 10;
    } while (isdigit(peek(lexer)));
    return num;
}

// Just going to implement real base 10 for now
// TODO: Implement rest of numbers
void lexNumber(Lexer *lexer)
{
    setBookmark(lexer);
    int sign = lexSign(lexer);
    int lexedDigit = lexDigits(lexer);
    if (lexedDigit == -1)
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

    lexer->error = NO_ERROR;
    addToken(lexer, newIntToken(sign * lexedDigit));
    unsetBookmark(lexer);
    return;
}

void lexPeculiarIdentifier(Lexer *lexer)
{
    char *str = calloc(MAX_IDENTIFIER_LENGTH, sizeof(char));
    matchDelimitedString(lexer, "+");
    if (lexer->error == NO_ERROR)
    {
        str[0] = '+';
        addToken(lexer, newIdentifierToken(str));
        lexer->error = NO_ERROR;
        return;
    }
    matchDelimitedString(lexer, "-");
    if (lexer->error == NO_ERROR)
    {
        str[0] = '-';
        addToken(lexer, newIdentifierToken(str));
        lexer->error = NO_ERROR;
        return;
    }
    matchDelimitedString(lexer, "...");
    if (lexer->error == NO_ERROR)
    {
        for (int i = 0; i <= 2; i++)
            str[i] = '.';
        addToken(lexer, newIdentifierToken(str));
        lexer->error = NO_ERROR;
        return;
    }
    free(str);
}

void lexIdentifier(Lexer *lexer)
{
    setBookmark(lexer);
    char c = peek(lexer);
    char *str = calloc(MAX_IDENTIFIER_LENGTH, sizeof(char));
    if (!(isAlphabet(c) || isSpecialInitial(c)))
    {
        lexer->error = STRING_MATCH_FAILURE;
        free(str);
        return;
    }
    str[0] = nextChar(lexer);
    for (int i = 1, c = peek(lexer); 
            (i < MAX_IDENTIFIER_LENGTH)
            && (isAlphabet(c) || isSpecialSubsequent(c) || isdigit(c))
            && hasNext(lexer);
            i++, c = peek(lexer))
        str[i] = nextChar(lexer);

    if (!isDelimiter(peek(lexer)))
    {
        lexer->error = EXPECTING_DELIMITER;
        returnToBookmark(lexer);
        free(str);
        return;
    }
    unsetBookmark(lexer);
    lexer->error = NO_ERROR;
    addToken(lexer, newIdentifierToken(str));
}

void lexSingleCharToken(Lexer *lexer, char match, SimpleToken SIMPLE_TOKEN)
{
    char c = peek(lexer);
    if (c != match)
    {
        lexer->error = STRING_MATCH_FAILURE;
        return;
    }
    lexer->error = NO_ERROR;
    nextChar(lexer);
    addToken(lexer, newSimpleToken(SIMPLE_TOKEN));
    return;
}

void lexSingleToken(Lexer *lexer)
{
    lexSingleCharToken(lexer, '(', LOPEN_PAREN);
    if (lexer->error == NO_ERROR)
        return;

    lexSingleCharToken(lexer, ')', LCLOSE_PAREN);
    if (lexer->error == NO_ERROR)
        return;

    lexSingleCharToken(lexer, '\'', LQUOTE);
    if (lexer->error == NO_ERROR)
        return;

    lexNumber(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexBoolean(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexCharacter(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexIdentifier(lexer);
    if (lexer->error == NO_ERROR)
        return;

    lexPeculiarIdentifier(lexer);
    if (lexer->error == NO_ERROR)
        return;
}

void lexIntertokenSpace(Lexer *lexer)
{
    while (lexSpaces(lexer) || lexComment(lexer))
        /* ignore spaces and comments for now */ ;
}

void lexAllInput(Lexer *lexer)
{
    while (lexer->error == NO_ERROR && hasNext(lexer))
    {
        lexSingleToken(lexer);
        lexIntertokenSpace(lexer);
        // if (hasNext(lexer))
        // printf("hasNext: true\n");
        // else
        // printf("hasNext: false\n");
        if (lexer->error != NO_ERROR)
            break;
    }
    reverseTokens(&(lexer->tokens));
}
