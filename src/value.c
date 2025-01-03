#include <common.h>
#include <stdio.h>
#include <assert.h>

#include "orion_memory.h"
#include "value.h"

void initValueArr(ValueArr* valueArr) {
    valueArr->count = 0;
    valueArr->capacity = DEFAULT_VALUE_ARR_CAPACITY;
    valueArr->data = (Value*)malloc(sizeof(Value) * DEFAULT_VALUE_ARR_CAPACITY);
}

void pushValueArrEl(ValueArr* valueArr, Value new_el) {
    assert(!isValueArrFull(valueArr) && "ValueArr is full.");

    if (valueArr->count == valueArr->capacity) {
        assert((valueArr->capacity * 2 <= UINT8_MAX) && "ValueArr can't grow more.");
        valueArr->capacity *= 2;
        valueArr->data = GROW_ARRAY(Value, valueArr->data, valueArr->capacity);
    }

    valueArr->data[valueArr->count] = new_el;
    valueArr->count++;
}

Value popValueArrEl(ValueArr* valueArr) {
    assert(valueArr->count > 0 && "valueArr is empty");

    Value popped_el = valueArr->data[valueArr->count - 1];
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
        printf("%lf ", AS_NUMBER(valueArr->data[i]));
    }
    printf("\n");
}

bool isValueArrFull(ValueArr* valueArr) {
    return valueArr->count == UINT8_MAX;
}
