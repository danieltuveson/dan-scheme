#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

Lexer *newLexer(char *input)
{
    int stringSize = strlen(input);
    Lexer *lexer = malloc(sizeof(Lexer));

    lexer->size = stringSize;
    lexer->charsLexed = 0;
    lexer->error = NO_ERROR;
    lexer->bookmark = 0;

    if (stringSize == 0) 
        lexer->hasNext = false;
    else
        lexer->hasNext = true;

    lexer->input = malloc(stringSize + 1);
    strcpy(lexer->input, input);

    return lexer;
}

void printLexer(Lexer *lexer)
{
    printf("input: %s\n", lexer->input);
    printf("charsLexed: %d\n", lexer->charsLexed);
    printf("size: %d\n", lexer->size);
    printf("hasNext: %d\n", lexer->hasNext);
    printf("bookmark: %d\n", lexer->bookmark);
    printf("error: %d\n", lexer->error);
}

void setBookmark(Lexer *lexer)
{
    lexer->bookmark = lexer->charsLexed;
}

// reset state of lexer (except for error state)
void returnToBookmark(Lexer *lexer)
{
    if (lexer->bookmark < lexer->charsLexed)
        lexer->hasNext = true;
    lexer->charsLexed = lexer->bookmark;
}

// nextChar: grabs next character. If there's an error, returns null and sets
// the lexer error value
char nextChar(Lexer *lexer)
{
    assert(lexer->charsLexed <= lexer->size);
    if (!lexer->hasNext)
    {
        lexer->error = UNEXPECTED_END_OF_INPUT;
        return '\0';
    }
    else
    {
        char c = lexer->input[lexer->charsLexed];
        lexer->charsLexed++;
        if (lexer->charsLexed == lexer->size)
            lexer->hasNext = false;
        return c;
    }
}

void deleteLexer(Lexer *lexer)
{
    free(lexer->input);
    free(lexer);
}

void matchString(Lexer *lexer, char *str)
{
    char c;
    setBookmark(lexer);
    for (int i = 0; i < strlen(str); i++)
    {
        if (lexer->hasNext)
        { 
            c = nextChar(lexer); 
            if (c != str[i]) 
            {
                lexer->error = STRING_MATCH_FAILURE;
                returnToBookmark(lexer);
                return;
            }
        }
        else
        {
            lexer->error = UNEXPECTED_END_OF_INPUT;
            returnToBookmark(lexer);
            return;
        }
    }
    lexer->error = NO_ERROR;
    return;
}

void lexBoolean(Lexer *lexer)
{
    matchString(lexer, "#f");
    if (lexer->error == NO_ERROR)
        return;

    matchString(lexer, "#t");
}
