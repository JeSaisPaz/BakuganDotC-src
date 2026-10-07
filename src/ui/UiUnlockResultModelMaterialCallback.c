// bdc 0x08937ce8 UiUnlockResultModelMaterialCallback
#include "bdc.h"

/* Material callback of the unlock-result reward model (`UiUnlockResultSetupReward`, reward kind 9
   branch, via `GfxModelForEachMaterial`): sets bits 5–7 of the material flag byte `+3` to 1
   (`0x20`) and bits 0–1 of flag byte `+4` to 2. */

void UiUnlockResultModelMaterialCallback(u8 *material)

{
  material[3] = material[3] & 0x1f | 0x20;
  material[4] = material[4] & 0xfc | 2;
  return;
}

