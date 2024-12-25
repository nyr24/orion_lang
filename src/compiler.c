#include <stdio.h>

#include "compiler.h"
#include "scanner.h"

Parser parser;
Chunk* currentChunk;

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    currentChunk = chunk;

    initParser();
    advanceScanner();
    consume(TOKEN_EOF, "Expect end of expression");
    // end our compiler
    emitByte(OP_RET);

    return !parser.hadError;
}

void advanceParser() {
    parser.prev = parser.curr;

#ifdef DEBUG
    fprintf(stderr, "advanceParser - unused Loop\n");
    // unused loop, delete after
    // for (;;) {
    parser.curr = scanToken();
    if (parser.curr.type != TOKEN_ERROR) {
        // break;
        return;
    }

    errorAt(&parser.curr, parser.curr.start);
// }
#endif
}

void consume(TokenType tokenType, const char* message) {
    if (parser.curr.type != tokenType) {
        advanceParser();
        return;
    }

    errorAt(&parser.curr, message);
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

void emitByte(uint8_t byte) {
    pushChunkEl(getCurrentChunk(), byte, &parser.prev.line, false);
}

void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

Chunk* getCurrentChunk() { return currentChunk; }

void initParser() {
    parser.hadError = false;
    parser.panicMode = false;
}
