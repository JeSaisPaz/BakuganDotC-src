// bdc 0x089f8a88 GfxDeferredDeleteFlush
#include "bdc.h"

/* Deletes every object queued in the deferred-delete list (`0x08b02c70`) through its virtual
   destructor (slot 1, flags 3). Called once per frame from `CoreTaskManagerUpdate`, so objects
   are destroyed outside the code that still draws them. */

void GfxDeferredDeleteFlush(void)

{
  CoreObject *obj;
  CoreObject *next;
  const VtblEntry *entry;

  obj = g_gfxDeferredDeleteHead;
  if (obj != (CoreObject *)0x0) {
    do {
      entry = &((const VtblEntry *)obj->vtable)[1];
      next = obj->next;
      ((void (*)(void *, s32))entry->fn)((u8 *)obj + entry->delta, 3);
      obj = next;
    } while (obj != (CoreObject *)0x0);
  }
  return;
}
