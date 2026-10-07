// bdc 0x0889f8a4 GameGimmickCorePointMaterialSetBlendMode1
#include "bdc.h"

/* `GfxModelForEachMaterial` callback of the core-point gimmick (`GameGimmickCorePointCtor`):
   sets the blend bits 6–7 of the material flag byte `+4` to 1 (`(b & 0x3f) | 0x40`). */

void GameGimmickCorePointMaterialSetBlendMode1(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

