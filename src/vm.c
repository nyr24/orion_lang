#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>

#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "object.h"
#include "orion_string.h"
#include "orion_memory.h"
#include "value.h"
#include "vm.h"

#ifdef DEBUG
#include "debug.h"
#endif

VM vm;

void initVM() { initStack(&vm.stack); }

void initStack(Stack* stack) {
    stack->capacity = STACK_DEF_CAP;
    stack->count = 0;
    stack->data = malloc(sizeof(Value) * stack->capacity);
}

InterpretResult interpretChunk(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
#ifdef DEBUG
    printf("Compile error for source: %s\n", source);        
#endif
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->data;

    InterpretResult res = run();
    freeChunk(&chunk);

    return res;
}

InterpretResult run() {
// util macros
#define BINARY_OP(RESULT_VAL, op)   \
    do {    \
        if (!IS_NUMBER(peekStack(&vm.stack, 0)) || !IS_NUMBER(peekStack(&vm.stack, 1))) { \
            runtimeError("Operands must be numbers.");  \
            return INTERPRET_RUNTIME_ERROR;             \
        }                                               \
        double b = AS_NUMBER(popStack(&vm.stack));     \
        double a = AS_NUMBER(popStack(&vm.stack));     \
        pushStack(&vm.stack, RESULT_VAL(a op b));       \
    } while (false)

#define BINARY_LOGIC_OP(op) \
    do { \
        Value b = popStack(&vm.stack); \
        Value a = popStack(&vm.stack); \
        pushStack(&vm.stack, BOOL_VAL(toBool(a) op toBool(b))); \
    } while (false)

#define THROW_IF_NAN(val)       \
    do {                        \
        if (!IS_NUMBER(val)) {  \
            runtimeError("Operand must be a number.");      \
            return INTERPRET_RUNTIME_ERROR;                 \
        }                       \
    } while (false)
// util macros end
    for (;;) {
        uint8_t instruction = *(vm.ip);
#ifdef DEBUG
        showStack(&vm.stack);
        int offset = (int)(vm.ip - vm.chunk->data);
        disassembleInstruction(vm.chunk, offset);
#endif
        vm.ip++;
        switch (instruction) {
            case OP_RET: {
                Value ret = popStack(&vm.stack);
                switch (ret.type) {
                    case VAL_BOOL:
                        printf("%s\n", AS_BOOL(ret) ? "true" : "false");
                        break;
                    case VAL_NIL:
                        printf("nil\n");
                        break;
                    case VAL_NUMBER:
                        printf("%lf\n", AS_NUMBER(ret));
                        break;
                    case VAL_OBJ:
                        // TODO: temp code
                        printf("object\n");
                        break;
                }
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = vm.chunk->constants.data[*vm.ip];
                vm.ip++;
                pushStack(&vm.stack, constant);
                break;
            }
            case OP_TRUE: {
                pushStack(&vm.stack, BOOL_VAL(true));
                break;
            }
            case OP_FALSE: {
                pushStack(&vm.stack, BOOL_VAL(false));
                break;
            }
            case OP_NIL: {
                pushStack(&vm.stack, NIL_VAL);
                break;
            }
            case OP_NEGATE: {
                Value* val = peekStackReference(&vm.stack, 0);
                THROW_IF_NAN(*val);
                AS_NUMBER(*val) *= -1;
                break;
            }
            case OP_INC: {
                Value* val = peekStackReference(&vm.stack, 0);
                THROW_IF_NAN(*val);
                AS_NUMBER(*val)++;
                break;
            }
            case OP_DEC: {
                Value* val = peekStackReference(&vm.stack, 0);
                THROW_IF_NAN(*val);
                AS_NUMBER(*val)--;
                break;
            }
            case OP_NOT: {
                pushStack(&vm.stack, BOOL_VAL(isFalseyValue(popStack(&vm.stack))));
                break;
            }
            case OP_AND: {
                BINARY_LOGIC_OP(&&);
                break;
            }
            case OP_OR: {
                BINARY_LOGIC_OP(||);
                break;
            }
            case OP_XOR: {
                BINARY_LOGIC_OP(^);
                break;
            }
            case OP_EQUAL: {
                Value b = popStack(&vm.stack);
                Value a = popStack(&vm.stack);
                pushStack(&vm.stack, BOOL_VAL(areValuesEqual(a, b)));
                break;
            }
            case OP_NOT_EQUAL: {
                Value b = popStack(&vm.stack);
                Value a = popStack(&vm.stack);
                pushStack(&vm.stack, BOOL_VAL(!areValuesEqual(a, b)));
                break;
            }
            case OP_GREATER_EQUAL: {
                BINARY_OP(BOOL_VAL, >);
                break;
            }
            case OP_GREATER: {
                BINARY_OP(BOOL_VAL, >);
                break;
            }
            case OP_LESS_EQUAL: {
                BINARY_OP(BOOL_VAL, <=);
                break;
            }
            case OP_LESS: {
                BINARY_OP(BOOL_VAL, <);
                break;
            }
            case OP_ADD: {
                if (IS_STRING(peekStack(&vm.stack, 0)) && IS_STRING(peekStack(&vm.stack, 1))) {
                    ObjString* result = strConcat(AS_STRING(popStack(&vm.stack)), AS_STRING(popStack(&vm.stack)));
                    pushStack(&vm.stack, OBJ_VAL(result));
                }
                else if (IS_NUMBER(peekStack(&vm.stack, 0)) && IS_NUMBER(peekStack(&vm.stack, 1))) {
                    BINARY_OP(NUMBER_VAL, +);
                }
                else {
                    runtimeError("operands must be either numbers or strings\n");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUB: {
                BINARY_OP(NUMBER_VAL, -);
                break;
            }
            case OP_MULT: {
                BINARY_OP(NUMBER_VAL, *);
                break;
            }
            case OP_DIV: {
                BINARY_OP(NUMBER_VAL, /);
                break;
            }
            }
        }

#undef BINARY_OP
}

void freeVM() {
    free(vm.stack.data);
    vm.stack.data = NULL;

    freeObjects();
}

void freeObjects() {
    Obj* curr = vm.objects;
    Obj* next;

    while (curr != NULL) {
        next = curr->next;
        switch (curr->type) {
            case OBJ_STRING: {
                ObjString* str = (ObjString*)curr;
                strFree(str);
                break;
            }
            case OBJ_INSTANCE: {
                // TODO: temporary code
                free(curr);
                break;
            }
        }
        curr = next;
    }
}

void pushStack(Stack* stack, Value value) {
    if (isStackFull(stack)) {
        stack->capacity *= 2;
        stack->data = GROW_ARRAY(Value, stack->data, stack->capacity);
    }

    stack->data[stack->count] = value;
    stack->count++;
}

Value popStack(Stack* stack) {
    assert(!isStackEmpty(stack) && "stack is empty");

    stack->count--;
    return stack->data[stack->count];
}

Value peekStack(Stack* stack, int distance) {
    assert(!isStackEmpty(stack) && "stack is empty");

    return stack->data[stack->count - 1 - distance];
}

Value* peekStackReference(Stack* stack, int distance) { 
    return stack->data + (stack->count - 1 - distance);
}

bool isStackEmpty(Stack* stack) { return stack->count == 0; }
bool isStackFull(Stack* stack) { return stack->count == stack->capacity; }

void showStack(Stack* stack) {
    for (Value* slot = stack->data; slot != (stack->data + stack->count); ++slot) {
        printf("[ ");
        switch (slot->type) {
            case VAL_BOOL:
                if (AS_BOOL(*slot)) {
                    printf("true");
                }
                else {
                    printf("false");
                }
                break;
            case VAL_NIL:
                printf("nil");
                break;
            case VAL_NUMBER:
                printf("%lf", AS_NUMBER(*slot));
                break;
            case VAL_OBJ:
                if (IS_STRING(*slot)) {
                    printf("%s", AS_CSTRING(*slot));
                }
                break;
            default:
                printf("unrecognized");
        }
        printf(" ]\n");
    }

    printf("\n");
}

void resetStack(Stack* stack) {
    stack->count = 0;
}

void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputc('\n', stderr);

    size_t instruction = (vm.ip - vm.chunk->data) - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack(&vm.stack);
}

