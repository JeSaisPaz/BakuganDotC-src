// bdc 0x089e3774 GfxSetActiveCamera
#include "bdc.h"

/* Deletes the current active camera (`g_gfxActiveCamera`) and installs `cam` instead, creating a
   new default camera (`GfxCameraCtor`) when `cam` is NULL. Returns the active camera. */

void *GfxSetActiveCamera(void *cam)

{
  bool fromLow;
  CoreNode *newCam;

  if (g_gfxActiveCamera != (GfxCamera *)0x0) {
    const VtblEntry *e = &((const VtblEntry *)g_gfxActiveCamera->base.vtable)[1];
    ((void (*)(void *, s32))e->fn)((char *)g_gfxActiveCamera + e->delta, 3);
    g_gfxActiveCamera = (GfxCamera *)0x0;
  }
  if (cam == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    newCam = MemAlloc(sizeof(GfxCamera), (const char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    cam = (void *)0x0;
    if (newCam != (CoreNode *)0x0) {
      GfxCameraCtor(newCam);
      cam = newCam;
    }
  }
  g_gfxActiveCamera = (GfxCamera *)cam;
  return cam;
}
