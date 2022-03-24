void matchString(Lexer *lexer, char *str);
void matchAnyChar(Lexer *lexer);
bool oneOf(char c, char *str);
void matchDelimiter(Lexer *lexer);
void matchDelimitedString(Lexer *lexer, char *string);
void lexBoolean(Lexer *lexer);
void lexSingleCharacter(Lexer *lexer);
void lexCharacter(Lexer *lexer);
