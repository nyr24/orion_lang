#include <memory.h>
#include <assert.h>
#include "orion_string.h"
#include "object.h"
#include "orion_memory.h"

AllocResult allocStrDataFromSrc(const char* src, int32_t length, int32_t capacity) {
    assert(capacity > length && "capacity shouldn't be smaller than length");

    char* newStrData    = ALLOCATE(char, capacity);
    memcpy(newStrData, src, length);
    newStrData[length]  = '\0';

    AllocResult res;
    res.data            = newStrData;
    res.length          = length;
    res.capacity        = capacity;

    return res;
}

AllocResult allocStrDataFromTwoSrc(const char* src1, int32_t length1, const char* src2, int32_t length2) {
    const int32_t length    = length1 + length2;
    const int32_t capacity  = length + 1;
    char* newStrData        = ALLOCATE(char, capacity);
    memcpy(newStrData , src1, length1);
    memcpy(newStrData  + length1, src2, length2);
    newStrData[length]      = '\0';

    AllocResult res;
    res.data        = newStrData;
    res.length      = length;
    res.capacity    = capacity;

    return res;
}

ObjString* strNew(const char* start, int32_t length) {
    return strWithCapacity(start, length, length + 1);
}

ObjString* strWithCapacity(const char* start, int32_t length, int32_t capacity) {
    assert(capacity > length && "capacity shouldn't be smaller than length");

    AllocResult allocRes    = allocStrDataFromSrc(start, length, capacity);
    ObjString* string       = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length          = allocRes.length;
    string->capacity        = allocRes.capacity;
    string->data            = allocRes.data;

    return string;
}

ObjString* strFromData(char* heap_data, int32_t length, int32_t capacity) {
    ObjString* string   = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->data        = heap_data;
    string->length      = length;
    string->capacity    = capacity;

    return string;
}

ObjString* strConcat(ObjString* first, ObjString* second) {
    AllocResult allocRes    = allocStrDataFromTwoSrc(first->data, first->length, second->data, second->length);
    strFree(first);
    strFree(second);
    ObjString* resultString = strFromData(allocRes.data, allocRes.length, allocRes.capacity);
    return resultString;
}

ObjString* strAppend(ObjString* dest, const char* append_seq_start, int32_t append_seq_len) {
    if (dest->capacity < (dest->length + append_seq_len + 1)) {
        AllocResult allocRes    = allocStrDataFromTwoSrc(dest->data, dest->length, append_seq_start, append_seq_len);
        strFree(dest);
        ObjString* resultString = strFromData(allocRes.data, allocRes.length, allocRes.capacity);
        return resultString;
    }
    else {
        memcpy(dest + dest->length, append_seq_start, append_seq_len);
        return dest;
    }
}

void strFree(ObjString* str) {
    if (str != NULL) {
        free(str->data);
        free(str);
        str = NULL;
    }
}
