#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
InterpretResult interpretChunk(Chunk* chunk);
InterpretResult run();
void freeVM();
void pushStack(Value value);
Value popStack();
Value peekStack();
bool isStackFull();
bool isStackEmpty();
void showStack();

#endif
