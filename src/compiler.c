#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "chunk.h"
#include "compiler.h"
#include "scanner.h"
#include "debug.h"
#include "value.h"

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
    [TOKEN_BANG]          = {unary,    NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     binary, PREC_AND},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     binary, PREC_OR},
    [TOKEN_XOR]           = {NULL,     binary, PREC_OR},
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
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
    emitConstant(NUMBER_VAL(value));
}

void literal() {
    switch (parser.prev.type) {
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        default:
            assert(true && "Unrecognized literal presented.");
    }
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
        case TOKEN_BANG:
            emitByte(OP_NOT);
            break;
        default:
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
        case TOKEN_AND:
            emitByte(OP_AND);
            break;
        case TOKEN_OR:
            emitByte(OP_OR);
            break;
        case TOKEN_XOR:
            emitByte(OP_XOR);
            break;
        case TOKEN_EQUAL_EQUAL:
            emitByte(OP_EQUAL);
            break;
        case TOKEN_BANG_EQUAL:
            emitByte(OP_NOT_EQUAL);
            break;
        case TOKEN_GREATER_EQUAL:
            emitByte(OP_GREATER_EQUAL);
            break;
        case TOKEN_GREATER:
            emitByte(OP_GREATER);
            break;
        case TOKEN_LESS_EQUAL:
            emitByte(OP_LESS_EQUAL);
            break;
        case TOKEN_LESS:
            emitByte(OP_LESS);
            break;
        default:
            return;  // Unreachable.
    }
}

// utils
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
