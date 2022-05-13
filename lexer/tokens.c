#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "tokens.h"

Token *newBoolToken(LBoolean b)
{
    Token *token = malloc(sizeof(Token));
    token->type = LBOOLEAN;
    token->value.lbool = b;
    return token;
}

Token *newCharToken(char c)
{
    Token *token = malloc(sizeof(Token));
    token->type = LCHARACTER;
    token->value.lchar = c;
    return token;
}

Token *newIntToken(int i)
{
    Token *token = malloc(sizeof(Token));
    token->type = LINT;
    token->value.lint = i;
    return token;
}

Token *newIdentifierToken(char *str)
{
    Token *token = malloc(sizeof(Token));
    token->type = LIDENTIFIER;
    token->value.lidentifier = str;
    return token;
}

Token *newSimpleToken(SimpleToken SIMPLE_TOKEN)
{
    Token *token = malloc(sizeof(Token));
    token->type = LSIMPLE_TOKEN;
    token->value.lsimpletoken = SIMPLE_TOKEN;
    return token;
}

Tokens *newTokens(void)
{
    Tokens *tokens = malloc(sizeof(Tokens));
    tokens->token = NULL;
    tokens->next = NULL;
    return tokens;
}

Tokens *push(Tokens *tokens, Token *token)
{
    Tokens *newTok = newTokens();
    newTok->token = token;
    newTok->next = tokens;
    return newTok;
}

Token *pop(Tokens **tokens_ptr)
{
    assert(tokens_ptr != NULL);
    assert(*tokens_ptr != NULL);
    Token *token = (*tokens_ptr)->token;
    *tokens_ptr = (*tokens_ptr)->next;
    return token;
}

void reverseTokens(Tokens **tokens_ptr)
{
    assert(tokens_ptr != NULL);
    assert(*tokens_ptr != NULL);
    // Tokens **original = tokens_ptr;
    Token *token;
    Tokens *tokens = NULL;
    while (*tokens_ptr != NULL)
    {
        token = pop(tokens_ptr);
        tokens = push(tokens, token);
    }
    *tokens_ptr = tokens;
    // deleteTokens
    // probably need to fix memory management of tokens a bit
}

void deleteTokens(Tokens *tokens)
{
    Tokens *old;
    while (tokens != NULL)
    {
        free(tokens->token);
        old = tokens;
        tokens = tokens->next;
        free(old);
    }
}

void printSimpleToken(SimpleToken t)
{
    switch (t)
    {
        case LOPEN_PAREN:
            printf("(");
            break;
        case LCLOSE_PAREN:
            printf(")");
            break;
        case LQUOTE:
            printf("'");
            break;
    }
}

void printToken(Token *token)
{
    if (token == NULL)
    {
        printf("NULL TOKEN");
        return;
    }
    switch (token->type)
    {
        case LBOOLEAN:
            if (token->value.lbool == LTrue)
                printf("#t");
            else
                printf("#f");
            break;
        case LINT:
            printf("%i", token->value.lint);
            break;
        case LCHARACTER:
            printf("#\\%c", token->value.lchar);
            break;
        case LIDENTIFIER:
            printf("%s", token->value.lidentifier);
            break;
        case LSIMPLE_TOKEN:
            printSimpleToken(token->value.lsimpletoken);
            break;
        default:
            printf("TOKEN TYPE NOT RECOGNIZED");
    }
}

void printlnToken(Token *token)
{
    printToken(token);
    printf("\n");
}

void printTokens(Tokens *tokens)
{
    while (tokens != NULL)
    {
        printToken(tokens->token);
        printf(" -> ");
        tokens = tokens->next;
    }
    printf("END OF INPUT");
    printf("\n");
}

void printError(LexError err)
{
    switch (err)
    {
        case NO_ERROR:
            printf("NO_ERROR");
            break;
        case UNEXPECTED_END_OF_INPUT:
            printf("UNEXPECTED_END_OF_INPUT");
            break;
        case STRING_MATCH_FAILURE:
            printf("STRING_MATCH_FAILURE");
            break;
        case EXPECTING_DELIMITER:
            printf("EXPECTING_DELIMITER");
            break;
    }
    printf("\n");
}
