#include "object.h"
#include "orion_memory.h"
#include "vm.h"

Obj* allocateObject(size_t size, ObjType type) {
    Obj* obj = reallocate(NULL, size);
    obj->type = type;
    obj->next = vm.objects;
    vm.objects = obj;
    return obj;
}
