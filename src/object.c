#include "object.h"
#include "orion_memory.h"

ObjString* copyString(const char* start, int32_t length) {
    char* newString = ALLOCATE(char, length + 1);
    memcpy(newString, start, length);
    newString[length] = '\0';

    return allocateString(newString, length);
}

Obj* allocateObject(size_t size, ObjType type) {
    Obj* obj = reallocate(NULL, size);
    obj->type = type;
    return obj;
}

ObjString* allocateString(char* start, int32_t length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->data = start;
    string->length = length;
    return string;
}

