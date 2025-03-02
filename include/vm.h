#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_DEF_CAP 256

typedef struct {
    Value*      data;
    uint32_t    count;
    uint32_t    capacity;
} Stack;

typedef struct {
    Stack       stack;
    Chunk*      chunk;
    uint8_t*    ip;
    // linked list of allocated objects
    Obj*        objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
InterpretResult interpretChunk(const char* source);
InterpretResult run();
void freeVM();
void freeObjects();
void initStack(Stack* stack);
void pushStack(Stack* stack, Value value);
Value popStack(Stack* stack);
Value peekStack(Stack* stack, int distance);
Value* peekStackReference(Stack* stack, int distance);
bool isStackFull(Stack* stack);
bool isStackEmpty(Stack* stack);
void showStack(Stack* stack);
void resetStack(Stack* stack);
void runtimeError(const char* format, ...);

extern VM vm;

#endif
