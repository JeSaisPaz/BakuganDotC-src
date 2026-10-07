// bdc 0x0895e804 UiEquipPulseGridCursor
#include "bdc.h"

/* Pulses the tint of the active grid cursor of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) (`UiPulseStepTint`, period 20): sprite `+0x516c`, or `+0x517a` while on the
   random button (`+0x75`); the pulse record is always the tween of `+0x516c`. */

void UiEquipPulseGridCursor(UiEquip *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  UiPulse *pulse = (UiPulse *)&self->tweens[self->spriteIdx[6]];

  if (self->onRandom == 0) {
    UiPulseStepTint(20.0f, sprites[self->spriteIdx[6]], pulse);
    return;
  }
  UiPulseStepTint(20.0f, sprites[self->spriteIdx[0xd]], pulse);
}
