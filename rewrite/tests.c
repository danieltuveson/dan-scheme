#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void tests()
{
    char c;
    int i;

    printf("...running tests...\n");

    printf("...testing newLexer...\n");

    char *testString = "hello, world!\n";
    Lexer *lexer = newLexer(testString);

    assert(lexer->error == NO_ERROR);
    assert(lexer->charsLexed == 0);
    assert(lexer->hasNext == true);
    assert(lexer->size == 14);
    assert(strcmp(lexer->input, "hello, world!\n") == 0);

    printf("...testing charsLexed...\n");
    assert(lexer->error == NO_ERROR);
    i = 0;
    c = nextChar(lexer);
    assert(c == 'h');
    assert(lexer->charsLexed == 1);

    for (i = 1; i < strlen("hello, world!\n"); i++)
    {
        c = nextChar(lexer);
        assert(c == lexer->input[i]);
    }
    assert(c == '\n');
    assert(lexer->hasNext == false);
    assert(lexer->error == NO_ERROR);
    nextChar(lexer);
    assert(lexer->error == UNEXPECTED_END_OF_INPUT);

    deleteLexer(lexer);
    lexer = newLexer(testString);

    printf("...testing matchString...\n");
    matchString(lexer, "hello");
    assert(lexer->error == NO_ERROR);
    matchString(lexer, "hello");
    assert(lexer->error == STRING_MATCH_FAILURE);
    matchString(lexer, ", world!\n");
    assert(lexer->hasNext == false);
    assert(lexer->error == NO_ERROR);

    nextChar(lexer);
    assert(lexer->error == UNEXPECTED_END_OF_INPUT);

    deleteLexer(lexer);

    printf("...testing lexBool...\n");
    lexer = newLexer("#f");
    lexBoolean(lexer);
    assert(lexer->error == NO_ERROR);
    deleteLexer(lexer);

    lexer = newLexer("#t");
    lexBoolean(lexer);
    assert(lexer->error == NO_ERROR);
    deleteLexer(lexer);

    lexer = newLexer("#g");
    lexBoolean(lexer);
    assert(lexer->error == STRING_MATCH_FAILURE);
    deleteLexer(lexer);

    printf("...finished!\n");
}
