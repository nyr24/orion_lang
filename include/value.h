#ifndef orion_value_h
#define orion_value_h

#include "common.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)

#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)

#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct {
    uint8_t count;
    uint32_t capacity;
    Value* data;
} ValueArr;

#define DEFAULT_VALUE_ARR_CAPACITY 16

void initValueArr(ValueArr* valueArr);
void pushValueArrEl(ValueArr* valueArr, Value new_el);
Value popValueArrEl(ValueArr* valueArr);
void freeValueArr(ValueArr* valueArr);
void printValueArr(ValueArr* valueArr);
bool isValueArrFull(ValueArr* valueArr);

#endif
