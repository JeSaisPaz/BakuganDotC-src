// bdc 0x08825874 GfxMeshObjCreateList2
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 2 (`0x08b00220`, drawn by `GfxMeshObjDrawList2`).
   Used by `GfxEffectRunCommands`. */

void * GfxMeshObjCreateList2(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList2);
  return obj;
}

