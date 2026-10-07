// bdc 0x08826330 GfxMeshObjDrawList4
#include "bdc.h"

/* Draws mesh-object list 4 (`0x08b00240`) without fog (`GfxMeshObjDrawList`). Called by
   `BtlMainDrawScene`. */

void GfxMeshObjDrawList4(void *packet)

{
  GfxMeshObjDrawList(packet,(GfxMeshObj *)g_gfxMeshObjList4.head,0);
  return;
}

