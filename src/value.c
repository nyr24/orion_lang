#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "value.h"
#include "orion_memory.h"

void initValueArr(ValueArr* valueArr) {
    valueArr->count = 0;
    valueArr->capacity = DEFAULT_VALUE_ARR_CAPACITY;
    valueArr->data = (Value*)malloc(sizeof(Value) * DEFAULT_VALUE_ARR_CAPACITY);
}

void pushValueArrEl(ValueArr* valueArr, Value new_el) {
    if (valueArr->count == valueArr->capacity) {
        uint32_t old_capacity = valueArr->capacity;
        valueArr->capacity *= 2;
        valueArr->data = GROW_ARRAY(Value, valueArr->data, old_capacity, valueArr->capacity);
    }

    valueArr->data[valueArr->count] = new_el;
    valueArr->count++;
}

Value popValueArrEl(ValueArr* valueArr) {
    if (valueArr->count == 0) {
        return -1;
    }

    Value popped_el = valueArr->data[valueArr->count - 1];
    valueArr->data[valueArr->count - 1] = 0;
    valueArr->count--;

    return popped_el;
}

void freeValueArr(ValueArr* valueArr) {
    if (valueArr->data != NULL) {
        free(valueArr->data);
        valueArr->data = NULL;
    }
}

void printValueArr(ValueArr* valueArr) {
    for (int i = 0; i < valueArr->count; ++i) {
        printf("%lf ", valueArr->data[i]);
    }
    printf("\n");
}
