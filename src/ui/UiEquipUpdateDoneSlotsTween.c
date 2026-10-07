// bdc 0x08961a28 UiEquipUpdateDoneSlotsTween
#include "bdc.h"

/* Advances the tweens started by `UiEquipStartDoneSlotsTween` on the Bakugan/gear loadout screen
   before a battle (task 302, `UiEquipCtor`): for every finished player (`doneCount`) it steps the
   sprite ranges `spriteIdx[0x17]/[0x18]`, `[0x2b]/[0x2c]` and `[0x3b]/[0x3c]` (start, per-player
   count) with `UiEquipUpdateDoneSlotSpriteTween` and re-aligns the second and third ranges to the
   first with `UiEquipAlignSpritesToAnchor`. Returns true when no player is done yet, otherwise
   true if at least one stepped sprite tween reported finished (sum of the per-sprite results != 0). */

bool UiEquipUpdateDoneSlotsTween(UiEquip *self, u8 out)
{
  int i;
  int idx;
  u8 player;
  u8 finished;

  if (self->doneCount == 0) {
    return true;
  }
  finished = 0;
  for (i = 0; i < self->doneCount; i++) {
    player = (u8)i;

    for (idx = self->spriteIdx[0x17] + self->spriteIdx[0x18] * player;
         idx < self->spriteIdx[0x17] + self->spriteIdx[0x18] * (player + 1); idx++) {
      finished = (u8)(finished + UiEquipUpdateDoneSlotSpriteTween(self, out, (u16)idx));
    }
    for (idx = self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * player;
         idx < self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * (player + 1); idx++) {
      finished = (u8)(finished + UiEquipUpdateDoneSlotSpriteTween(self, out, (u16)idx));
    }
    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x17], self->spriteIdx[0x18],
                                self->spriteIdx[0x2b], self->spriteIdx[0x2c], player);

    for (idx = self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * player;
         idx < self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * (player + 1); idx++) {
      finished = (u8)(finished + UiEquipUpdateDoneSlotSpriteTween(self, out, (u16)idx));
    }
    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x17], self->spriteIdx[0x18],
                                self->spriteIdx[0x3b], self->spriteIdx[0x3c], player);
  }
  return finished != 0;
}
