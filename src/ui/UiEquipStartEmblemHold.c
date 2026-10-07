// bdc 0x0895cc50 UiEquipStartEmblemHold
#include "bdc.h"

/* Arms a 15-frame hold before the emblem pop of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): stores 15 in byte `+0xb` of the emblem's tween record (`+0x78 + idx*0x28`, idx
   = `+0x5176`). */

void UiEquipStartEmblemHold(UiEquip *self)

{
  ((UiPulse *)&self->tweens[self->spriteIdx[0xb]])->waitFrames = 0x0f;
  return;
}

