// bdc 0x088258ec GfxMeshObjCreateList3
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 3 (`0x08b00230`, drawn by `GfxMeshObjDrawList3`).
   Used by `GfxMeshObjCreateSmokeColumn`. */

void * GfxMeshObjCreateList3(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList3);
  return obj;
}

