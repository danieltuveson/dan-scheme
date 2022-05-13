#include "tokenize.h"
#include "tokens.h"
#include "lexer.h"
#include "lexer_utils.h"

LexError tokenize(Tokens *tokens, char *text)
{
    Lexer *lexer = newLexer(text);
    lexAllInput(lexer);
    LexError err = lexer->error;
    tokens = lexer->tokens;
    deleteLexer(lexer);
    return err;
}
