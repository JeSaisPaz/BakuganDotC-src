// bdc 0x088258b0 GfxMeshObjCreateList1
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 1 (`0x08b00210`, drawn by `GfxMeshObjDrawList1`).
   Used by `GfxEffectRunCommands`. */

void * GfxMeshObjCreateList1(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList1);
  return obj;
}

