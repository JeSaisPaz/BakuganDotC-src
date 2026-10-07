// bdc 0x089e28e0 GfxCameraCtor
#include "bdc.h"

/* Constructor of the 0x2a0-byte camera object: `CoreNodeCtor` with no parent, camera vtable
   `0x08af54d4`, then the default settings (`GfxCameraInit`). */

CoreNode *GfxCameraCtor(CoreNode *cam)

{
  CoreNodeCtor(cam,(CoreNode *)0x0);
  cam->vtable = &g_gfxCameraVtbl;
  GfxCameraInit((GfxCamera *)cam);
  return cam;
}

