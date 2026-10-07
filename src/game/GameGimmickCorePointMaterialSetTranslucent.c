// bdc 0x0889f8bc GameGimmickCorePointMaterialSetTranslucent
#include "bdc.h"

/* `GfxModelForEachMaterial` callback of the core-point gimmick: sets bit 0x10 of the material
   flag byte `+4`, switching the material to translucent drawing while the core point fades (alpha
   `+0x178` below 1). */

void GameGimmickCorePointMaterialSetTranslucent(u8 *material)

{
  material[4] = material[4] | 0x10;
  return;
}

