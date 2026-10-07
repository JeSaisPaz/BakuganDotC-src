// bdc 0x08960470 UiEquipPulseGearCursor
#include "bdc.h"

/* Pulses the tint (`UiPulseStepTint`, period 20) of player `player`'s active equipment-panel
   cursor on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`): the list cursor
   `+0x51be + player` on row 0, the OK cursor `+0x51a6 + player` on row 1; the active row is
   `+0x5025` (in network mode — `SaveGetProfileFlag0` set — the local player's row
   `+0x5026[+0x52a0]`); row 0 = equipment list, row 1 = OK button. */

void UiEquipPulseGearCursor(UiEquip *self, u8 player)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  s8 row = self->activeRow;
  s32 idx;

  if (SaveGetProfileFlag0() != 0) {
    row = (s8)self->gearRowFlags[*(s32 *)self->localPlayer];
  }
  if (row <= 0) {
    if (row >= 0) {
      idx = self->spriteIdx[0x2f] + player;
      UiPulseStepTint(20.0f, sprites[idx], (UiPulse *)&self->tweens[idx]);
    }
  } else if (row < 2) {
    idx = self->spriteIdx[0x23] + player;
    UiPulseStepTint(20.0f, sprites[idx], (UiPulse *)&self->tweens[idx]);
  }
}
