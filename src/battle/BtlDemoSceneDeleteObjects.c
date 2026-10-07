// bdc 0x08904e1c BtlDemoSceneDeleteObjects
#include "bdc.h"

/* Deletes every object of a scene object list (`CoreObjectList`) through its virtual deleting
   destructor (vtable entry 1, flags 3); the next pointer is read before each delete. */
void BtlDemoSceneDeleteObjects(void *list)
{
  CoreObject *obj = ((CoreObjectList *)list)->head;
  CoreObject *next;
  const VtblEntry *dtor;

  while (obj != NULL) {
    next = obj->next;
    dtor = &((const VtblEntry *)obj->vtable)[1];
    ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
    obj = next;
  }
}
