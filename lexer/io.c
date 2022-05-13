#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "lexer_utils.h"

void mainLoop(void)
{
    while (true)
    {
        printf("> ");

        Lexer *lexer = lexReplInput();
        printf("======================\n");

        printf("### Lexer Content ###\n");
        printLexer(lexer);
        printf("======================\n");

        printf("### Tokens Parsed ###\n");
        printTokens(lexer->tokens);
        printf("======================\n");

        if (lexer->error != NO_ERROR)
        {
            printf("### Error ###\n");
            printError(lexer->error);
            printf("======================\n");

            fflush(stdin);
            deleteTokens(lexer->tokens);
            deleteLexer(lexer);
        }
        printf("\n");
    }
}

Lexer *lexReplInput(void)
{
    const int BUFF_SIZE = 10000;

    char c, *buff;
    buff = malloc(sizeof(char) * BUFF_SIZE);
    c = fgetc(stdin);
    
    int i;
    for (i = 0; c != '\n'; i++, c = fgetc(stdin))
    {
        if (i == BUFF_SIZE)
        {
            printf("Exceeded maximum size of %d characters for a word\n", BUFF_SIZE);
            return NULL;
        }
        // else if (!is_word_char(c))
        // {
        //     printf("Error: '%c' not expected at this time\n", c);
        //     return NULL;
        // }
        else
        {
            buff[i] = c;
        }
    }
    Lexer *lexer = newLexer(buff);
    lexAllInput(lexer);
    free(buff);
    return lexer;
}

bool is_word_char(char c)
{
    if (strchr("()`'#@ \t\r\n", c))
        return false;
    else
        return true;
}

