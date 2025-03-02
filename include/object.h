#ifndef orion_object_h
#define orion_object_h

#include "common.h"
#include "value.h"

typedef enum {
    OBJ_STRING,
    OBJ_INSTANCE
} ObjType;

struct Obj {
    ObjType     type;
    Obj*        next;
};

Obj*  allocateObject(size_t size, ObjType type);

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define ALLOCATE_OBJ(type, objType) \
    (type*)allocateObject(sizeof(type), objType)

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
