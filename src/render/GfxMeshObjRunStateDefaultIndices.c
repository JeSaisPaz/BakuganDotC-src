// bdc 0x088256e0 GfxMeshObjRunStateDefaultIndices
#include "bdc.h"

/* Runs the mesh object (`GfxMeshObjCtor`)'s state handler (`GfxMeshObjRunState`) and then resets
   its index pointer `+0xf4` to the built-in index table `0x08a625ee`. Used by
   `GfxEffectRunCommands`. */

void GfxMeshObjRunStateDefaultIndices(GfxMeshObj *self)

{
  GfxMeshObjRunState(self);
  self->indices = g_gfxMeshObjDefaultIndices;
  return;
}

