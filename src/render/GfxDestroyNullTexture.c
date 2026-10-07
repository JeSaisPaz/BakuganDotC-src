// bdc 0x089f6c20 GfxDestroyNullTexture
#include "bdc.h"

/* Deletes the `"NonTexture"` fallback texture `g_nullTexture` through its virtual destructor
   (flags 3) and clears the pointer. Counterpart of `GfxInitNullTexture`, called from
   `CoreTaskManagerDestroy`. */

typedef struct GfxVtblEntry {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *self, s32 flags);
} GfxVtblEntry;

typedef struct GfxVtbl {
  u32 hdr[2];
  GfxVtblEntry dtor;
} GfxVtbl;

void GfxDestroyNullTexture(void)
{
  CoreObject *obj = (CoreObject *)g_nullTexture;
  const GfxVtblEntry *entry;

  if (obj != (CoreObject *)0x0) {
    entry = &((const GfxVtbl *)obj->vtable)->dtor;
    entry->fn((u8 *)obj + entry->thisAdjust, 3);
    g_nullTexture = (void *)0x0;
  }
}
