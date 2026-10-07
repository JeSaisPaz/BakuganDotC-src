// bdc 0x089f8a88 GfxDeferredDeleteFlush
#include "bdc.h"

/* Deletes every object queued in the deferred-delete list (`0x08b02c70`) through its virtual
   destructor (slot 1, flags 3). Called once per frame from `CoreTaskManagerUpdate`, so objects
   are destroyed outside the code that still draws them. */

typedef struct GfxVtblEntry {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *self, s32 flags);
} GfxVtblEntry;

typedef struct GfxVtbl {
  u32 hdr[2];
  GfxVtblEntry dtor;
} GfxVtbl;

void GfxDeferredDeleteFlush(void)

{
  CoreObject *obj;
  CoreObject *next;
  const GfxVtblEntry *entry;

  obj = g_gfxDeferredDeleteHead;
  if (obj != (CoreObject *)0x0) {
    do {
      entry = &((const GfxVtbl *)obj->vtable)->dtor;
      next = obj->next;
      entry->fn((u8 *)obj + entry->thisAdjust, 3);
      obj = next;
    } while (obj != (CoreObject *)0x0);
  }
  return;
}
