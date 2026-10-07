// bdc 0x08825838 GfxMeshObjCreateList5
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 5 (`0x08b00250`, drawn by `GfxMeshObjDrawList5`).
   Used by `GfxEffectRunCommands`. */

void * GfxMeshObjCreateList5(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList5);
  return obj;
}

