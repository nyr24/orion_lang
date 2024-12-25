#ifndef orion_parser_h
#define orion_parser_h
#include "vm.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_INTERPOLATION,
    TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FUN,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,

    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void repl();
void runFile(const char* path);
char* readFile(const char* path);
void initScanner(const char* source);
Token scanToken();
bool isAtEnd();
Token makeToken(TokenType tokenType);
Token errorToken(const char* message);
char advanceScanner();
bool match(char expected);
void skipWhitespaceAndComments();
char peek();
char peekNext();
Token string();
Token number();
Token identifier();
Token interpolation();
TokenType identifierType();
TokenType checkKeyword(int offset, int length, const char* rest,
                       TokenType type);
bool isDigit(char c);
bool isAlpha(char c);

#endif
