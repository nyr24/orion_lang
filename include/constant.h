#ifndef orion_value_h
#define orion_value_h

#include "common.h"

typedef double Constant;

typedef struct {
    uint32_t count;
    uint32_t capacity;
    Constant* data;
} ConstantArr;

#define DEFAULT_CONSTANT_ARR_CAPACITY 8

void initConstantArr(ConstantArr* constantArr);
void pushConstantArrEl(ConstantArr* constantArr, Constant new_el);
Constant popConstantArrEl(ConstantArr* constantArr);
void freeConstantArr(ConstantArr* constantArr);
void printConstantArr(ConstantArr* constantArr);

#endif
