// bdc 0x088257fc GfxMeshObjCreateList0
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 0 (`0x08b00200`, drawn by `GfxMeshObjDrawList0`).
   Used by `GfxEffectRunCommands`. */

void * GfxMeshObjCreateList0(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList0);
  return obj;
}

