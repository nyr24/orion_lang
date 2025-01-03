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

void initVM(VM* vm);
InterpretResult interpretChunk(VM* vm, const char* source);
InterpretResult run(VM* vm);
void freeVM(VM* vm);
void initStack(Stack* stack);
void pushStack(Stack* stack, Value value);
Value popStack(Stack* stack);
Value peekStack(Stack* stack, int distance);
Value* peekStackReference(Stack* stack, int distance);
bool isStackFull(Stack* stack);
bool isStackEmpty(Stack* stack);
void showStack(Stack* stack);
void resetStack(Stack* stack);
void runtimeError(VM* vm, const char* format, ...);

#endif
