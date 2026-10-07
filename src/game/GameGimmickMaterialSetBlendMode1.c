// bdc 0x088d8f84 GameGimmickMaterialSetBlendMode1
#include "bdc.h"

/* `GfxModelForEachMaterial` callback installed on every material of a gimmick model by
   `GameGimmickEnableMaterialCallback`: sets the blend bits 6–7 of the material flag byte `+4`
   to 1 (`(b & 0x3f) | 0x40`). */

void GameGimmickMaterialSetBlendMode1(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

