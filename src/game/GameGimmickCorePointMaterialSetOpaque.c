// bdc 0x0889f8cc GameGimmickCorePointMaterialSetOpaque
#include "bdc.h"

/* `GfxModelForEachMaterial` callback of the core-point gimmick: clears bit 0x10 of the material
   flag byte `+4`, returning the material to opaque drawing once the fade alpha `+0x178` reaches 1.
    */

void GameGimmickCorePointMaterialSetOpaque(u8 *material)

{
  material[4] = material[4] & 0xef;
  return;
}

