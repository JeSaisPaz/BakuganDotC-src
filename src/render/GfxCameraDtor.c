// bdc 0x089e2920 GfxCameraDtor
#include "bdc.h"

/* Destructor of the camera object: restores the camera vtable, clears the screen-camera
   (`0x08ac5c8c`) and active-camera (`0x08ac5c90`) globals if they point to it, runs
   `CoreNodeDtor` and frees it when `flags & 1`. */

void GfxCameraDtor(CoreNode *cam, u32 flags)

{
  if (cam != (CoreNode *)0x0) {
    cam->vtable = g_gfxCameraVtbl;
    if (g_gfxActiveCamera == (GfxCamera *)cam) {
      g_gfxActiveCamera = (GfxCamera *)0x0;
    }
    if (g_gfxScreenCamera == (GfxCamera *)cam) {
      g_gfxScreenCamera = (GfxCamera *)0x0;
    }
    CoreNodeDtor(cam,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(cam,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

