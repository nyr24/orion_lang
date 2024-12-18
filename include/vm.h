#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_DEF_CAP 256

typedef struct {
    uint32_t count;
    uint32_t capacity;
    Value* data;
} Stack;

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Stack stack;
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
Value* peekStackReference();
bool isStackFull();
bool isStackEmpty();
void showStack();

#endif
