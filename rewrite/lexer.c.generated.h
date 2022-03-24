Lexer *newLexer(char *input);
void deleteLexer(Lexer *lexer);
void printLexer(Lexer *lexer);
void setBookmark(Lexer *lexer);
void unsetBookmark(Lexer *lexer);
void returnToBookmark(Lexer *lexer);
char nextChar(Lexer *lexer);
