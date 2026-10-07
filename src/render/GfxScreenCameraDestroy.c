// bdc 0x089e3720 GfxScreenCameraDestroy
#include "bdc.h"

/* Deletes the screen camera `g_gfxScreenCamera` through its virtual destructor (vtable entry with
   this-adjust and function pointer) and clears the pointer; called by `GfxRenderShutdown`. */

void GfxScreenCameraDestroy(void)
{
  if (g_gfxScreenCamera != 0) {
    const VtblEntry *e = &((const VtblEntry *)g_gfxScreenCamera->base.vtable)[1];

    ((void (*)(void *, s32))e->fn)((char *)g_gfxScreenCamera + e->delta, 3);
    g_gfxScreenCamera = 0;
  }
}
