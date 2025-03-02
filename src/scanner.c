#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "scanner.h"
#include "vm.h"

Scanner scanner;

Token scanToken() {
    skipWhitespaceAndComments();
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advanceScanner();
    if (isAlpha(c)) {
        return scanIdentifier();
    }
    if (isDigit(c)) {
        return scanNumber();
    }

    switch (c) {
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);
        case '*':
            return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return scanString();
        case '$':
            return scanInterpolation();
    }

    return errorToken("Unexpected character.");
}

void repl() {
    char line[1024];

    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpretChunk(line);
    }
}

void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpretChunk(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        fclose(file);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        fclose(file);
        exit(74);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

bool isAtEnd() { return (*scanner.current) == '\0'; }

Token makeToken(TokenType tokenType) {
    Token token;
    token.type = tokenType;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

char advanceScanner() {
    scanner.current++;
    return scanner.current[-1];
}

bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

void skipWhitespaceAndComments() {
    for (;;) {
        char c = peek();
        switch (c) {
                // whitespace
            case '\t':
            case '\r':
            case ' ':
                advanceScanner();
                break;
            case '\n':
                scanner.line++;
                advanceScanner();
                break;
                // comments
            case '/':
                if (peekNext() == '/') {
                    while (!isAtEnd() && peek() != '\n') {
                        advanceScanner();
                    }
                }
                else {
                    return;
                }
                
                break;
            default:
                return;
        }
    }
}

char peek() { return *scanner.current; }
char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

Token scanString() {
    while (peek() != '"' && !isAtEnd()) {
        char c = advanceScanner();
        if (c == '\n') {
            scanner.line++;
        }
    }

    if (isAtEnd()) {
        return errorToken("Unterminated string");
    }

    advanceScanner();
    return makeToken(TOKEN_STRING);
}

Token scanNumber() {
    while (isDigit(peek())) {
        advanceScanner();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        do {
            advanceScanner();
        } while (isDigit(peek()));
    }

    return makeToken(TOKEN_NUMBER);
}

Token scanIdentifier() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advanceScanner();
    }

    return makeToken(identifierType());
}

TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a':
            return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c':
            return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h':
                        return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'x':
            return checkKeyword(1, 2, "or", TOKEN_XOR);
        case 'w':
            return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        default:
            return TOKEN_IDENTIFIER;
    }

    return TOKEN_IDENTIFIER;
}

TokenType checkKeyword(int offset, int length, const char* rest,
                       TokenType type) {
    if ((scanner.current - scanner.start == offset + length) &&
        memcmp(scanner.start + offset, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

Token scanInterpolation() {
    if (peek() != '{') {
        return errorToken("Unfinished interpolation syntax");
    }

    while (peek() != '}' && !isAtEnd()) {
        char c = advanceScanner();
        if (c == '\n') {
            scanner.line++;
        }
    }

    if (isAtEnd()) {
        return errorToken("Unterminated interpolation");
    }

    return makeToken(TOKEN_INTERPOLATION);
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
