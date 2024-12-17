#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

VM vm;

void resetStack() { vm.stackTop = vm.stack; }

void initVM() { resetStack(); }

InterpretResult interpretChunk(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->data;
    return run();
}

InterpretResult run() {
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
            printf("%lf\n", popStack());
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
            printf("%lf (long)\n", constant);
            break;
        }
        case OP_NEGATE: {
            pushStack(-popStack());
            break;
        }
        }
    }
}

void freeVM() {}

void pushStack(Value value) {
    if (isStackFull()) {
        return;
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

Value popStack() {
    if (isStackEmpty()) {
        return -1;
    }

    vm.stackTop--;
    return *vm.stackTop;
}

Value peekStack() {
    if (isStackEmpty()) {
        return -1;
    }

    return *(vm.stackTop - 1);
}

bool isStackEmpty() { return vm.stackTop == vm.stack; }
bool isStackFull() { return vm.stackTop == (vm.stack + STACK_MAX + 1); }

void showStack() {
    for (Value* slot = vm.stack; slot < vm.stackTop; ++slot) {
        printf("[ ");
        printf("%lf", *slot);
        printf(" ]\n");
    }

    printf("\n");
}
