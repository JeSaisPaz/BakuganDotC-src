// bdc 0x089e3720 GfxScreenCameraDestroy
#include "bdc.h"

/* Deletes the screen camera `g_gfxScreenCamera` through its virtual destructor (vtable entry with
   this-adjust and function pointer) and clears the pointer; called by `GfxRenderShutdown`. */

typedef struct GfxDtorEntry {
  short thisAdjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxDtorEntry;

typedef struct GfxDtorVtable {
  unsigned char head[8];
  GfxDtorEntry dtor;
} GfxDtorVtable;

void GfxScreenCameraDestroy(void)
{
  if (g_gfxScreenCamera != 0) {
    const GfxDtorEntry *e = &((const GfxDtorVtable *)g_gfxScreenCamera->base.vtable)->dtor;

    e->fn((char *)g_gfxScreenCamera + e->thisAdjust, 3);
    g_gfxScreenCamera = 0;
  }
}
