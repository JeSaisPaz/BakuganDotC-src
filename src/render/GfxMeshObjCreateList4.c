// bdc 0x08825928 GfxMeshObjCreateList4
#include "bdc.h"

/* `GfxMeshObjCreate` + append to draw list 4 (`0x08b00240`, drawn without fog by
   `GfxMeshObjDrawList4`). Used by `GfxEffectRunCommands`. */

void * GfxMeshObjCreateList4(s32 state, void *owner)

{
  CoreObject *obj;
  
  obj = GfxMeshObjCreate(state,owner);
  CoreObjectListAppend(obj,&g_gfxMeshObjList4);
  return obj;
}

