#include "constant.h"
#include "orion_memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void initConstantArr(ConstantArr* constantArr) {
    constantArr->count = 0;
    constantArr->capacity = DEFAULT_CONSTANT_ARR_CAPACITY;
    constantArr->data =
        (Constant*)malloc(sizeof(Constant) * DEFAULT_CONSTANT_ARR_CAPACITY);
}

void pushConstantArrEl(ConstantArr* constantArr, Constant new_el) {
    if (constantArr->count == constantArr->capacity) {
        uint32_t old_capacity = constantArr->capacity;
        constantArr->capacity *= 2;
        constantArr->data = GROW_ARRAY(Constant, constantArr->data,
                                       old_capacity, constantArr->capacity);
    }

    constantArr->data[constantArr->count] = new_el;
    constantArr->count++;
}

Constant popConstantArrEl(ConstantArr* constantArr) {
    if (constantArr->count == 0) {
        return -1;
    }

    Constant popped_el = constantArr->data[constantArr->count - 1];
    constantArr->data[constantArr->count - 1] = 0;
    constantArr->count--;

    return popped_el;
}

void freeConstantArr(ConstantArr* constantArr) {
    if (constantArr->data != NULL) {
        free(constantArr->data);
        constantArr->data = NULL;
    }
}

void printConstantArr(ConstantArr* constantArr) {
    for (int i = 0; i < constantArr->count; ++i) {
        printf("%lf ", constantArr->data[i]);
    }
    printf("\n");
}
