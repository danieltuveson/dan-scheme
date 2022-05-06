#include <stdio.h>
#include <string.h>
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
    RUN_TEST(lexNumber);

    printf("... finished!\n");
}

void test_newLexer()
{
    char *testString = "hello, world!\n";
    Lexer *lexer = newLexer(testString);

    assert(lexer->error == NO_ERROR);
    assert(lexer->charsLexed == 0);
    assert(hasNext(lexer));
    assert(lexer->inputBookmark->value == 0);
    assert(lexer->inputBookmark->prevBookmark == NULL);
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
    assert(!hasNext(lexer));
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
    assert(!hasNext(lexer));
    assert(lexer->error == NO_ERROR);

    nextChar(lexer);
    assert(lexer->error == UNEXPECTED_END_OF_INPUT);

    deleteLexer(lexer);
}

void test_lexBool()
{
    Lexer *lexer;

    lexer = newLexer("#f)");
    lexBoolean(lexer);
    assert(lexer->error == NO_ERROR);
    assert(lexer->token != NULL);
    assert(lexer->token->type == LBOOLEAN);
    assert(lexer->token->value.lbool == LFalse);
    deleteLexer(lexer);

    lexer = newLexer("#t(");
    lexBoolean(lexer);
    assert(lexer->error == NO_ERROR);
    assert(lexer->token != NULL);
    assert(lexer->token->type == LBOOLEAN);
    assert(lexer->token->value.lbool == LTrue);
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

void test_lexNumber()
{
    Lexer *lexer;

    lexer = newLexer("hey");
    lexNumber(lexer);
    assert(lexer->error == STRING_MATCH_FAILURE);
    deleteLexer(lexer);

    lexer = newLexer("10395 ;");
    lexNumber(lexer);
    assert(lexer->error == NO_ERROR);
    assert(lexer->token != NULL);
    assert(lexer->token->type == LINT);
    assert(lexer->token->value.lint == 10395);
    deleteLexer(lexer);

    lexer = newLexer("-10395\n");
    lexNumber(lexer);
    assert(lexer->error == NO_ERROR);
    assert(lexer->token != NULL);
    assert(lexer->token->type == LINT);
    assert(lexer->token->value.lint == -10395);
    deleteLexer(lexer);

    // lexer = newLexer("5/67");
    // lexNumber(lexer);
    // assert(lexer->error == NO_ERROR);
    // deleteLexer(lexer);

    // lexer = newLexer("+5.67e11");
    // lexNumber(lexer);
    // assert(lexer->error == NO_ERROR);
    // deleteLexer(lexer);

    // lexer = newLexer("-5.67###");
    // lexNumber(lexer);
    // assert(lexer->error == NO_ERROR);
    // deleteLexer(lexer);
}
