#ifndef orion_compiler_h
#define orion_compiler_h

#include <stdint.h>

#include "chunk.h"
#include "scanner.h"
#include "value.h"

typedef struct {
    Token prev;
    Token curr;
    bool hadError;
    bool panicMode;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

bool compile(const char* source, Chunk* chunk);
void consume(TokenType tokenType, const char* message);
void advanceParser();
void emitByte(uint8_t byte);
void emitBytes(uint8_t byte1, uint8_t byte2);
void emitConstant(Value value);
// compiling expressions
void number();
void string();
void literal();
void expression();
void unary();
void binary();
void grouping();
// util
void endCompiler();
void errorAt(Token* token, const char* message);
void initParser();
Chunk* getCurrentChunk();
uint8_t makeConstant(Value value);
void parsePrecedence(Precedence prec);
ParseRule* getRule(TokenType type);

#endif
