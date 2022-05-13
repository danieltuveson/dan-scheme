#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/* This file provides core functionality of lexer e.g. initialization, 
 * deleting, printing, and frequent methods for updating lexer state. 
 * More complex additional functionality should be added to lexer_utils
 */

Lexer *newLexer(char *input)
{
    int stringSize = strlen(input);
    Lexer *lexer = malloc(sizeof(Lexer));

    lexer->size = stringSize;
    lexer->charsLexed = 0;
    lexer->error = NO_ERROR;

    lexer->inputBookmark = malloc(sizeof(Bookmark));
    lexer->inputBookmark->value = 0;
    lexer->inputBookmark->prevBookmark = NULL;
    lexer->tokens = NULL;

    lexer->input = malloc(stringSize + 1);
    strcpy(lexer->input, input);

    return lexer;
}

void deleteLexer(Lexer *lexer)
{
    Bookmark *current, *prev;
    current = lexer->inputBookmark;
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

    Bookmark *b = lexer->inputBookmark;
    printf("inputBookmark: %lu", b->value);
    while (b->prevBookmark != NULL)
    {
        b = b->prevBookmark;
        printf("->%lu", b->value);
    }
    printf("\n");

    printf("error: %d\n", lexer->error);
}
// Push new value of inputBookmark onto the stack
void setBookmark(Lexer *lexer)
{
    if (lexer->charsLexed == 0)
        return;
    
    Bookmark *inputBookmark = malloc(sizeof(Bookmark));
    inputBookmark->value = lexer->charsLexed;
    inputBookmark->prevBookmark = lexer->inputBookmark;
    lexer->inputBookmark = inputBookmark;
}

// Pop last value of inputBookmark from the stack
void unsetBookmark(Lexer *lexer)
{
    // Don't unset initial inputBookmark
    if (lexer->inputBookmark->prevBookmark == NULL)
        return;

    Bookmark *b = lexer->inputBookmark->prevBookmark;
    free(lexer->inputBookmark);
    lexer->inputBookmark = b;
}

// Reset state of lexer (except for error state) and pop value of inputBookmark off 
// of the inputBookmark stack
void returnToBookmark(Lexer *lexer)
{
    lexer->charsLexed = lexer->inputBookmark->value;
    unsetBookmark(lexer);
}

// nextChar: grabs next character. If there's an error, returns null and sets 
// the lexer error value
char nextChar(Lexer *lexer)
{
    if (!hasNext(lexer))
    {
        lexer->error = UNEXPECTED_END_OF_INPUT;
        return '\0';
    }
    char c = lexer->input[lexer->charsLexed];
    lexer->charsLexed++;
    return c;
}

