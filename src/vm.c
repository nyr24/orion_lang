#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "orion_memory.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

VM vm;

void initStack() {
    vm.stack.capacity = STACK_DEF_CAP;
    vm.stack.count = 0;
    vm.stack.data = malloc(sizeof(Value) * vm.stack.capacity);
}

void initVM() { initStack(); }

InterpretResult interpretChunk(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->data;

    InterpretResult res = run();
    freeChunk(&chunk);

    return res;
}

InterpretResult run() {
#define BINARY_OP(op)                                                          \
    do {                                                                       \
        Value b = popStack();                                                  \
        Value a = popStack();                                                  \
        pushStack(a op b);                                                     \
    } while (false)

    for (;;) {
        uint8_t instruction = *vm.ip;
#ifdef DEBUG_TRACE_EXECUTION
        showStack();
        int offset = (int)(vm.ip - vm.chunk->data);
        disassembleInstruction(vm.chunk, offset);
#endif
        vm.ip++;
        switch (instruction) {
        case OP_RET: {
            Value ret = popStack();
            printf("%lf\n", ret);
            return INTERPRET_OK;
        }
        case OP_CONSTANT: {
            Value constant = vm.chunk->constants.data[*vm.ip];
            vm.ip++;
            pushStack(constant);
            break;
        }
        case OP_CONSTANT_LONG: {
            Value constant = vm.chunk->constants.data[*vm.ip];
            vm.ip++;
            pushStack(constant);
            break;
        }
        case OP_NEGATE: {
            Value* val = peekStackReference();
            *val *= -1;
            break;
        }
        case OP_INC: {
            Value* val = peekStackReference();
            (*val)++;
            break;
        }
        case OP_DEC: {
            Value* val = peekStackReference();
            (*val)--;
            break;
        }
        case OP_ADD: {
            BINARY_OP(+);
            break;
        }
        case OP_SUB: {
            BINARY_OP(-);
            break;
        }
        case OP_MULT: {
            BINARY_OP(*);
            break;
        }
        case OP_DIV: {
            BINARY_OP(/);
            break;
        }
        }
    }

#undef BINARY_OP
}

void freeVM() {
    free(vm.stack.data);
    vm.stack.data = NULL;
}

void pushStack(Value value) {
    if (isStackFull()) {
        vm.stack.capacity *= 2;
        vm.stack.data = GROW_ARRAY(Value, vm.stack.data, vm.stack.capacity);
    }

    vm.stack.data[vm.stack.count] = value;
    vm.stack.count++;
}

Value popStack() {
    if (isStackEmpty()) {
        return -1;
    }

    vm.stack.count--;
    return vm.stack.data[vm.stack.count];
}

Value peekStack() {
    if (isStackEmpty()) {
        return -1;
    }

    return vm.stack.data[vm.stack.count - 1];
}

Value* peekStackReference() { return vm.stack.data + (vm.stack.count - 1); }

bool isStackEmpty() { return vm.stack.count == 0; }
bool isStackFull() { return vm.stack.count == vm.stack.capacity; }

void showStack() {
    for (Value* slot = vm.stack.data; slot != (vm.stack.data + vm.stack.count);
         ++slot) {
        printf("[ ");
        printf("%lf", *slot);
        printf(" ]\n");
    }

    printf("\n");
}
