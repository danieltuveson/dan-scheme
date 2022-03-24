#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "lexer.h"

Lexer *newLexer(char *input)
{
    int stringSize = strlen(input);
    Lexer *lexer = malloc(sizeof(Lexer));

    lexer->size = stringSize;
    lexer->charsLexed = 0;
    lexer->error = NO_ERROR;

    lexer->bookmark = malloc(sizeof(Bookmark));
    lexer->bookmark->value = 0;
    lexer->bookmark->prevBookmark = NULL;

    if (stringSize == 0) 
        lexer->hasNext = false;
    else
        lexer->hasNext = true;

    lexer->input = malloc(stringSize + 1);
    strcpy(lexer->input, input);

    return lexer;
}

void deleteLexer(Lexer *lexer)
{
    Bookmark *current, *prev;
    current = lexer->bookmark;
    while (current != NULL)
    {
        prev = current->prevBookmark;
        free(current);
        current = prev;
    }
    free(lexer->input);
    free(lexer);
}

void printLexer(Lexer *lexer)
{
    printf("input: %s\n", lexer->input);
    printf("charsLexed: %lu\n", lexer->charsLexed);
    printf("size: %lu\n", lexer->size);
    printf("hasNext: %d\n", lexer->hasNext);

    Bookmark *b = lexer->bookmark;
    printf("bookmark: %lu", b->value);
    while (b->prevBookmark != NULL)
    {
        b = b->prevBookmark;
        printf("->%lu", b->value);
    }
    printf("\n");

    printf("error: %d\n", lexer->error);
}

// Push new value of bookmark onto the stack
void setBookmark(Lexer *lexer)
{
    if (lexer->charsLexed == 0)
        return;
    
    Bookmark *bookmark = malloc(sizeof(Bookmark));
    bookmark->value = lexer->charsLexed;
    bookmark->prevBookmark = lexer->bookmark;
    lexer->bookmark = bookmark;
}

// Pop last value of bookmark from the stack
void unsetBookmark(Lexer *lexer)
{
    // Don't unset initial bookmark
    if (lexer->bookmark->prevBookmark == NULL)
        return;

    Bookmark *b = lexer->bookmark->prevBookmark;
    free(lexer->bookmark);
    lexer->bookmark = b;
}

// Reset state of lexer (except for error state) and pop value of bookmark off 
// of the bookmark stack
void returnToBookmark(Lexer *lexer)
{
    if (lexer->bookmark->value < lexer->charsLexed)
        lexer->hasNext = true;

    lexer->charsLexed = lexer->bookmark->value;
    unsetBookmark(lexer);
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
    char c = lexer->input[lexer->charsLexed];
    lexer->charsLexed++;
    if (lexer->charsLexed == lexer->size)
        lexer->hasNext = false;
    return c;
}
