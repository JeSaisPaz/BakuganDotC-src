// bdc 0x0882627c GfxMeshObjDrawList0
#include "bdc.h"

/* Draws mesh-object list 0 (`0x08b00200`) with fog (`GfxMeshObjDrawList`). Called by
   `BtlMainDrawScene`, `GameFieldDrawScene` and the battle demo draw. */

void GfxMeshObjDrawList0(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList0.head,1);
  return;
}

