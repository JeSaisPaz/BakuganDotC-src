// bdc 0x088262a0 GfxMeshObjDrawList5
#include "bdc.h"

/* Draws mesh-object list 5 (`0x08b00250`) with fog (`GfxMeshObjDrawList`). Called by
   `GameFieldScreenFxDraw`. */

void GfxMeshObjDrawList5(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList5.head,1);
  return;
}

