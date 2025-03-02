#include <common.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "orion_memory.h"
#include "value.h"
#include "object.h"

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
        switch (valueArr->data[i].type) {
            case VAL_NUMBER:
                printf("%lf\n", AS_NUMBER(valueArr->data[i]));
                break;
            case VAL_BOOL:
                printf("%s\n", AS_BOOL(valueArr->data[i]) ? "true" : "false");
                break;
            case VAL_NIL:
                printf("%s\n", "nil");
                break;
            case VAL_OBJ:
                printObject(valueArr->data[i]);
                break;
        }
    }
}

void printObject(Value value) {
    switch (AS_OBJ(value)->type) {
        case OBJ_STRING:
            printf("%s\n", AS_CSTRING(value));
            break;
        case OBJ_INSTANCE:
            // TODO:
            break;
    }
}

bool isValueArrFull(ValueArr* valueArr) {
    return valueArr->count == UINT8_MAX;
}

bool isFalseyValue(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value))
        || (IS_NUMBER(value) && AS_NUMBER(value) == 0);
}

bool toBool(Value val) {
    return !isFalseyValue(val);
}

bool areValuesEqual(Value a, Value b) {
    if (a.type != b.type) {
        return false;
    }

    switch (a.type) {
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_OBJ: {
            ObjString* string1 = AS_STRING(a);
            ObjString* string2 = AS_STRING(b);
            return  string1->length == string2->length &&
                    memcmp(string1->data, string2->data, string1->length) == 0;
        }
        case VAL_NIL: return true;
    }
}
