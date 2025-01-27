#ifndef orion_object_h
#define orion_object_h

#include "common.h"
#include "orion_memory.h"
#include "value.h"
#include <memory.h>

typedef enum {
    OBJ_STRING,
    OBJ_INSTANCE
} ObjType;

struct Obj {
    ObjType type;
};

struct ObjString {
    char*       data;
    int32_t     length;
    Obj         obj_base;
};

ObjString* copyString(const char* start, int32_t length);
ObjString* allocateString(char* start, int32_t length);
Obj* allocateObject(size_t size, ObjType type);

#define OBJ_TYPE(value)             (AS_OBJ(value)->type)

#define IS_STRING(value)            (isObjType(value, OBJ_STRING))

#define AS_STRING(value)            ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)           (((ObjString*)AS_OBJ(value))->data)

#define ALLOCATE_OBJ(type, objType) \
    (type*)allocateObject(sizeof(type), objType)

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
