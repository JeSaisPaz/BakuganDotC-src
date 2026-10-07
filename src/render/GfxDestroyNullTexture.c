// bdc 0x089f6c20 GfxDestroyNullTexture
#include "bdc.h"

/* Deletes the `"NonTexture"` fallback texture `g_nullTexture` through its virtual destructor
   (flags 3) and clears the pointer. Counterpart of `GfxInitNullTexture`, called from
   `CoreTaskManagerDestroy`. */

void GfxDestroyNullTexture(void)
{
  CoreObject *obj = (CoreObject *)g_nullTexture;
  const VtblEntry *entry;

  if (obj != (CoreObject *)0x0) {
    entry = &((const VtblEntry *)obj->vtable)[1];
    ((void (*)(void *, s32))entry->fn)((u8 *)obj + entry->delta, 3);
    g_nullTexture = (void *)0x0;
  }
}
