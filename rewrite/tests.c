#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "tests.h"
#include "lexer.h"
#include "lexer_utils.h"

void tests()
{
    printf("... running tests ...\n");

    RUN_TEST(newLexer);
    RUN_TEST(charLexed);
    RUN_TEST(matchString);
    RUN_TEST(lexBool);
    RUN_TEST(lexCharacter);

    printf("... finished!\n");
}

void test_newLexer()
{
    char *testString = "hello, world!\n";
    Lexer *lexer = newLexer(testString);

    assert(lexer->error == NO_ERROR);
    assert(lexer->charsLexed == 0);
    assert(lexer->hasNext == true);
    assert(lexer->bookmark->value == 0);
    assert(lexer->bookmark->prevBookmark == NULL);
    assert(lexer->size == 14);
    assert(strcmp(lexer->input, "hello, world!\n") == 0);
}

void test_charLexed()
{
    char c;
    unsigned int i;
    char *testString = "hello, world!\n";
    Lexer *lexer = newLexer(testString);

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
}

void test_matchString()
{
    char *testString = "hello, world!\n";
    Lexer *lexer = newLexer(testString);

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
}

void test_lexBool()
{
    Lexer *lexer;

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
}

void test_lexCharacter()
{
    Lexer *lexer;

    lexer = newLexer("#\\g");
    lexCharacter(lexer);
    assert(lexer->error == NO_ERROR);
    deleteLexer(lexer);

    lexer = newLexer("#\\space");
    lexCharacter(lexer);
    assert(lexer->error == NO_ERROR);
    deleteLexer(lexer);

    lexer = newLexer("#\\newline");
    lexCharacter(lexer);
    assert(lexer->error == NO_ERROR);
    deleteLexer(lexer);

    lexer = newLexer("#\\soup");
    lexCharacter(lexer);
    assert(lexer->error == STRING_MATCH_FAILURE);
}
