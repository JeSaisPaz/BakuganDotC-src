// bdc 0x089e3774 GfxSetActiveCamera
#include "bdc.h"

/* Deletes the current active camera (`g_gfxActiveCamera`) and installs `cam` instead, creating a
   new default camera (`GfxCameraCtor`) when `cam` is NULL. Returns the active camera. */

typedef struct GfxVtable {
  unsigned char head[8];
  short adjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxVtable;

void *GfxSetActiveCamera(void *cam)

{
  bool fromLow;
  CoreNode *newCam;

  if (g_gfxActiveCamera != (GfxCamera *)0x0) {
    const GfxVtable *e = (const GfxVtable *)g_gfxActiveCamera->base.vtable;
    e->fn((char *)g_gfxActiveCamera + e->adjust, 3);
    g_gfxActiveCamera = (GfxCamera *)0x0;
  }
  if (cam == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    newCam = MemAlloc(0x2a0, (const char *)0x0, 0);
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
