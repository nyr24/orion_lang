#ifndef orion_value_h
#define orion_value_h

#include "common.h"

typedef double Value;

typedef struct {
    uint32_t    count;
    uint32_t    capacity;
    Value*      data;
} ValueArr;

#define DEFAULT_VALUE_ARR_CAPACITY 8

void    initValueArr(ValueArr* valueArr);
void    pushValueArrEl(ValueArr* valueArr, Value new_el);
Value   popValueArrEl(ValueArr* valueArr);
void    freeValueArr(ValueArr* valueArr);
void    printValueArr(ValueArr* valueArr);

#endif
