// bdc 0x088262e8 GfxMeshObjDrawList2
#include "bdc.h"

/* Draws mesh-object list 2 (`0x08b00220`) with fog (`GfxMeshObjDrawList`). Called by
   `GameFieldDrawScene`, the battle demo draw and `BtlMainDrawFlashAndCopy`. */

void GfxMeshObjDrawList2(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList2.head,1);
  return;
}

