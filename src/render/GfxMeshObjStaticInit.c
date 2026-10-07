// bdc 0x0882893c GfxMeshObjStaticInit
#include "bdc.h"

/* Static constructor of the mesh-object translation unit (`GfxMeshObjCtor`): writes the light-0
   position commands of the static GE light list at `0x08ab9e50` (`0x08ab9e64..0x08ab9e6c`): `LXP0`
   = -1.0, `LYP0` = -0.8, `LZP0` = -0.5 (float24 operands of commands 0x63/0x64/0x65). */

void GfxMeshObjStaticInit(void)

{
  g_gfxMeshObjLightList[5] = 0x63bf8000;
  g_gfxMeshObjLightList[6] = 0x64bf4ccc;
  g_gfxMeshObjLightList[7] = 0x65bf0000;
  return;
}

