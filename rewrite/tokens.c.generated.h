Token *newBoolToken(LBoolean b);
Token *newCharToken(char c);
Token *newIntToken(int i);
Token *newIdentifierToken(char *str);
Token *newSimpleToken(SimpleToken SIMPLE_TOKEN);
Tokens *newTokens(void);
Tokens *push(Tokens *tokens, Token *token);
Token *pop(Tokens **tokens_ptr);
void reverseTokens(Tokens **tokens_ptr);
void deleteTokens(Tokens *tokens);
void printSimpleToken(SimpleToken t);
void printToken(Token *token);
void printlnToken(Token *token);
void printTokens(Tokens *tokens);
void printError(LexError err);
