// bdc 0x08937d14 UiUnlockResultKind6MaterialCallback
#include "bdc.h"

/* Material callback of the unlock-result reward model for reward kind 6
   (`UiUnlockResultSetupReward`, after its specular setup, via `GfxModelForEachMaterial`):
   stores 0xb2 in the material's halfword `+0` and, when any of bits 5–7 of flag byte `+3` is set,
   sets all three. */

void UiUnlockResultKind6MaterialCallback(u8 *material)

{
  material[0] = 0xb2;
  material[1] = '\0';
  if (((int)(char)material[3] & 0xe0U) != 0) {
    material[3] = material[3] | 0xe0;
  }
  return;
}

