#ifndef orion_string_h
#define orion_string_h

#include "common.h"
#include "value.h"
#include "object.h"

struct ObjString {
    Obj         obj_base;
    char*       data;
    int32_t     length;
    int32_t     capacity;
};

typedef struct AllocResult {
    char*   data;
    int32_t length;
    int32_t capacity;
} AllocResult;

AllocResult allocStrDataFromSrc(const char* src, int32_t length, int32_t capacity);
AllocResult allocStrDataFromTwoSrc(const char* src1, int32_t length1, const char* src2, int32_t length2);
ObjString*  strNew(const char* start, int32_t length);
ObjString*  strWithCapacity(const char* start, int32_t length, int32_t capacity);
ObjString*  strFromData(char* heap_data, int32_t length, int32_t capacity);
ObjString*  strConcat(ObjString* first, ObjString* second);
ObjString*  strAppend(ObjString* dest, const char* append_seq_start, int32_t append_seq_len);
void        strFree(ObjString* str);

#define IS_STRING(value)            (isObjType(value, OBJ_STRING))
#define AS_STRING(value)            ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)           (((ObjString*)AS_OBJ(value))->data)

#endif
