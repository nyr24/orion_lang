#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "compiler.h"
#include "scanner.h"
#include "debug.h"

Parser parser;
Chunk* currentChunk;

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    currentChunk = chunk;

    initParser();
    advanceParser();
    expression();

    consume(TOKEN_EOF, "Expect end of expression");
    endCompiler();

    return !parser.hadError;
}

void advanceParser() {
    parser.prev = parser.curr;

    for (;;) {
        parser.curr = scanToken();
        if (parser.curr.type != TOKEN_ERROR) {
            break;
        }

        errorAt(&parser.curr, parser.curr.start);
    }
}

void consume(TokenType tokenType, const char* message) {
    if (parser.curr.type == tokenType) {
        advanceParser();
        return;
    }

    errorAt(&parser.curr, message);
}

void number() {
    double value = strtod(parser.prev.start, NULL);
    emitConstant(value);
}

void emitByte(uint8_t byte) {
    pushChunkEl(getCurrentChunk(), byte, &parser.prev.line, false);
}

void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

void emitConstant(Value constant) {
    pushConstantToChunk(getCurrentChunk(), constant, &parser.prev.line);
}

void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void unary() {
    TokenType operatorType = parser.prev.type;

    parsePrecedence(PREC_UNARY);

    switch (operatorType) {
        case TOKEN_MINUS:
            emitByte(OP_NEGATE);
            break;
        default:
            // todo
            return;
    }
}

void binary() {
    TokenType operatorType = parser.prev.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS:
            emitByte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emitByte(OP_SUB);
            break;
        case TOKEN_STAR:
            emitByte(OP_MULT);
            break;
        case TOKEN_SLASH:
            emitByte(OP_DIV);
            break;
        default:
            return;  // Unreachable.
    }
}

// utils
void errorAt(Token* token, const char* message) {
    if (parser.panicMode) {
        return;
    }
    parser.panicMode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end of file");
    } else if (token->type == TOKEN_ERROR) {
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

Chunk* getCurrentChunk() { return currentChunk; }

void initParser() {
    parser.hadError = false;
    parser.panicMode = false;
}

void parsePrecedence(Precedence prec) {
    advanceParser();
    ParseFn prefixRule = getRule(parser.prev.type)->prefix;

    if (prefixRule == NULL) {
        errorAt(&parser.prev, "Expected expression.");
        return;
    }

    prefixRule();

    while (prec <= getRule(parser.curr.type)->precedence) {
        advanceParser();
        ParseFn infixRule = getRule(parser.prev.type)->infix;
        infixRule();
    }
}

void expression() { parsePrecedence(PREC_ASSIGNMENT); }

ParseRule* getRule(TokenType type) { return &rules[type]; }

void endCompiler() {
    emitByte(OP_RET);
#ifdef DEBUG
    if (!parser.hadError) {
        disassembleChunk(getCurrentChunk(), "code");
    }
#endif
}
