// bdc 0x0882630c GfxMeshObjDrawList3
#include "bdc.h"

/* Draws mesh-object list 3 (`0x08b00230`) with fog (`GfxMeshObjDrawList`). Called by
   `BtlMainDrawScene`. */

void GfxMeshObjDrawList3(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList3.head,1);
  return;
}

