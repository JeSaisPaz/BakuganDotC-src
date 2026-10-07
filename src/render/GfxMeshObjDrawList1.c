// bdc 0x088262c4 GfxMeshObjDrawList1
#include "bdc.h"

/* Draws mesh-object list 1 (`0x08b00210`) with fog (`GfxMeshObjDrawList`). Called by
   `BtlMainDrawScene`, `GameFieldDrawScene`, `BtlFinishTaskDraw`. */

void GfxMeshObjDrawList1(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList1.head,1);
  return;
}

