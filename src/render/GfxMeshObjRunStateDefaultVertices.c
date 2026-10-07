// bdc 0x088256e0 GfxMeshObjRunStateDefaultVertices
#include "bdc.h"

/* Runs the mesh object (`GfxMeshObjCtor`)'s state handler (`GfxMeshObjRunState`) and then resets
   its vertex pointer `vertices` (`+0xf4`) to the built-in vertex table `g_gfxMeshObjDefaultVertices`. Used by
   `GfxEffectRunCommands`. */

void GfxMeshObjRunStateDefaultVertices(GfxMeshObj *self)

{
  GfxMeshObjRunState(self);
  self->vertices = g_gfxMeshObjDefaultVertices;
  return;
}

