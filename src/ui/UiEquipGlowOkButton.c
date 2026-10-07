// bdc 0x08960558 UiEquipGlowOkButton
#include "bdc.h"

/* While the OK row is active, steps the cursor glow (`UiCursorGlowStep`) of player `player`'s OK
   button sprite `+0x519e + player` on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`); the active row is `+0x5025` (in network mode — `SaveGetProfileFlag0` set
   — the local player's row `+0x5026[+0x52a0]`); row 0 = equipment list, row 1 = OK button. */

void UiEquipGlowOkButton(UiEquip *self, u8 player)

{
  s8 row;

  row = self->activeRow;
  if (SaveGetProfileFlag0() != 0) {
    row = self->gearRowFlags[*(s32 *)self->localPlayer];
  }
  if (row == 1) {
    UiCursorGlowStep(((GfxSprite **)self->base.data)[self->spriteIdx[0x1f] + player]);
  }
  return;
}

