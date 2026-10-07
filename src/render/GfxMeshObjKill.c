// bdc 0x08825624 GfxMeshObjKill
#include "bdc.h"

/* Marks the mesh object (`GfxMeshObjCtor`) as dead (`+0x1c = 0xdead`) so its owner list drops it.
   Called by `GfxEffectDtor` for the effect's mesh object `+0x204`. */

void GfxMeshObjKill(GfxMeshObj *self)

{
  self->step = 0xdead;
  return;
}

